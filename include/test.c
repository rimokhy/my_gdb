//
// test.cpp for  in /home/rimokh_y/tracker
//
// Made by Yohan Rimokh
// Login   <rimokh_y@epitech.net>
//
// Started on  Wed May 18 03:17:10 2016 Yohan Rimokh
// Last update Wed May 18 03:29:45 2016 Yohan Rimokh
//

#include <assert.h>
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

// number of bytes in a JMP/CALL rel32 instruction
#define REL32_SZ 5

// copy in the string including the trailing null byte
static const char *format = "instruction pointer = %p\n";

// text seen in /proc/<pid>/maps for text areas
static const char *text_area = " r-xp ";

// this should be a string that will uniquely identify libc in /proc/<pid>/maps
static const char *libc_string = "/libc-2";

// Update the text area of pid at the area starting at where. The data copied
// should be in the new_text buffer whose size is given by len. If old_text is
// not null, the original text data will be copied into it. Therefore old_text
// must have the same size as new_text.
int poke_text(pid_t pid, void *where, void *new_text, void *old_text,
              size_t len) {
  if (len % sizeof(void *) != 0) {
    printf("invalid len, not a multiple of %zd\n", sizeof(void *));
    return -1;
  }

  long poke_data;
  for (size_t copied = 0; copied < len; copied += sizeof(poke_data)) {
    memmove(&poke_data, new_text + copied, sizeof(poke_data));
    if (old_text != NULL) {
      errno = 0;
      long peek_data = ptrace(PTRACE_PEEKTEXT, pid, where + copied, NULL);
      if (peek_data == -1 && errno) {
        perror("PTRACE_PEEKTEXT");
        return -1;
      }
      memmove(old_text + copied, &peek_data, sizeof(peek_data));
    }
    if (ptrace(PTRACE_POKETEXT, pid, where + copied, (void *)poke_data) < 0) {
      perror("PTRACE_POKETEXT");
      return -1;
    }
  }
  return 0;
}

int do_wait(const char *name) {
  int status;
  if (wait(&status) == -1) {
    perror("wait");
    return -1;
  }
  if (WIFSTOPPED(status)) {
    if (WSTOPSIG(status) == SIGTRAP) {
      return 0;
    }
    printf("%s unexpectedly got status %s\n", name, strsignal(status));
    return -1;
  }
  printf("%s got unexpected status %d\n", name, status);
  return -1;

}

int singlestep(pid_t pid) {
  if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL)) {
    perror("PTRACE_SINGLESTEP");
    return -1;
  }
  return do_wait("PTRACE_SINGLESTEP");
}

int32_t compute_jmp(void *from, void *to) {
  int64_t delta = (int64_t)to - (int64_t)from - REL32_SZ;
  if (delta < INT_MIN || delta > INT_MAX) {
    printf("cannot do relative jump of size %li; did you compile with -fPIC?\n",
           delta);
    exit(1);
  }
  return (int32_t)delta;
}

int fprintf_process(pid_t pid) {
  // attach to the process
  if (ptrace(PTRACE_ATTACH, pid, NULL, NULL)) {
    perror("PTRACE_ATTACH");
    check_yama();
    return -1;
  }

  // wait for the process to actually stop
  if (waitpid(pid, 0, WSTOPPED) == -1) {
    perror("wait");
    return -1;
  }

  // save the register state of the remote process
  struct user_regs_struct oldregs;
  if (ptrace(PTRACE_GETREGS, pid, NULL, &oldregs)) {
    perror("PTRACE_GETREGS");
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    return -1;
  }
  void *rip = (void *)oldregs.rip;
  printf("their %%rip           %p\n", rip);

  // First, we are going to allocate some memory for ourselves so we don't
  // need
  // to stop on the remote process' memory. We will do this by directly
  // invoking
  // the mmap(2) system call and asking for a single page.
  struct user_regs_struct newregs;
  memmove(&newregs, &oldregs, sizeof(newregs));
  newregs.rax = 60;

  uint8_t old_word[8];
  uint8_t new_word[8];
  new_word[0] = 0x0f; // SYSCALL
  new_word[1] = 0x05; // SYSCALL
  new_word[2] = 0xff; // JMP %rax
  new_word[3] = 0xe0; // JMP %rax

  // insert the SYSCALL instruction into the process, and save the old word
  if (poke_text(pid, rip, new_word, old_word, sizeof(new_word))) {
    goto fail;
  }

  // set the new registers with our syscall arguments
  if (ptrace(PTRACE_SETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_SETREGS");
    goto fail;
  }

  // invoke mmap(2)
  if (singlestep(pid)) {
    goto fail;
  }

  // read the new register state, so we can see where the mmap went
  if (ptrace(PTRACE_GETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_GETREGS");
    return -1;
  }

  uint8_t new_text[32];
  memset(new_text, 0, sizeof(new_text));

  // insert a CALL instruction
  size_t offset = 0;
  new_text[offset++] = 0xe8; // CALL rel32
  int32_t fprintf_delta = compute_jmp(mmap_memory, their_fprintf);
  memmove(new_text + offset, &fprintf_delta, sizeof(fprintf_delta));
  offset += sizeof(fprintf_delta);

  // insert a TRAP instruction
  new_text[offset++] = 0xcc;

  // copy our fprintf format string right after the TRAP instruction
  memmove(new_text + offset, format, strlen(format));

  // update the mmap area
  printf("inserting code/data into the mmap area at %p\n", mmap_memory);
  if (poke_text(pid, mmap_memory, new_text, NULL, sizeof(new_text))) {
    goto fail;
  }

  if (poke_text(pid, rip, new_word, NULL, sizeof(new_word))) {
    goto fail;
  }

  // set up our registers with the args to fprintf
  // memmove(&newregs, &oldregs, sizeof(newregs));
  newregs.rax = 0;                          // no vector registers are used
  newregs.rdi = (long)their_stderr;         // pointer to stderr in the caller
  newregs.rsi = (long)mmap_memory + offset; // pointer to the format string
  newregs.rdx = oldregs.rip;                // the integer we want to print

  printf("setting the registers of the remote process\n");
  if (ptrace(PTRACE_SETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_SETREGS");
    goto fail;
  }

  // continue the program, and wait for the trap
  printf("continuing execution\n");
  ptrace(PTRACE_CONT, pid, NULL, NULL);
  if (do_wait("PTRACE_CONT")) {
    goto fail;
  }

  if (ptrace(PTRACE_GETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_GETREGS");
    goto fail;
  }
  newregs.rax = (long)rip;
  if (ptrace(PTRACE_SETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_SETREGS");
    goto fail;
  }

  new_word[0] = 0xff; // JMP %rax
  new_word[1] = 0xe0; // JMP %rax
  poke_text(pid, (void *)newregs.rip, new_word, NULL, sizeof(new_word));

  printf("jumping back to original rip\n");
  if (singlestep(pid)) {
    goto fail;
  }
  if (ptrace(PTRACE_GETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_GETREGS");
    goto fail;
  }

  if (newregs.rip == (long)rip) {
    printf("successfully jumped back to original %%rip at %p\n", rip);
  } else {
    printf("unexpectedly jumped to %p (expected to be at %p)\n",
           (void *)newregs.rip, rip);
    goto fail;
  }

  // unmap the memory we allocated
  newregs.rax = 11;                // munmap
  newregs.rdi = (long)mmap_memory; // addr
  newregs.rsi = PAGE_SIZE;         // size
  if (ptrace(PTRACE_SETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_SETREGS");
    goto fail;
  }

  // make the system call
  printf("making call to mmap\n");
  if (singlestep(pid)) {
    goto fail;
  }
  if (ptrace(PTRACE_GETREGS, pid, NULL, &newregs)) {
    perror("PTRACE_GETREGS");
    goto fail;
  }
  printf("munmap returned with status %llu\n", newregs.rax);

  printf("restoring old text at %p\n", rip);
  poke_text(pid, rip, old_word, NULL, sizeof(old_word));

  printf("restoring old registers\n");
  if (ptrace(PTRACE_SETREGS, pid, NULL, &oldregs)) {
    perror("PTRACE_SETREGS");
    goto fail;
  }

  // detach the process
  printf("detaching\n");
  if (ptrace(PTRACE_DETACH, pid, NULL, NULL)) {
    perror("PTRACE_DETACH");
    goto fail;
  }
  return 0;

fail:
  poke_text(pid, rip, old_word, NULL, sizeof(old_word));
  if (ptrace(PTRACE_DETACH, pid, NULL, NULL)) {
    perror("PTRACE_DETACH");
  }
  return 1;
}
