//
// Breakpoints.hpp for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Thu May  5 05:23:37 2016 Yohan RIMOKH
// Last update Thu May 26 02:32:14 2016 Yohan Rimokh
//

#pragma once

#include "Info.hh"
#include <errno.h>
#include <string.h>

class	Breakpoints
{
private:
  void		*_oldText;
  Registers	_newRegs;
  Registers	_oldRegs;
  pid_t		_pid;
  void		*_addr;
public:
  Breakpoints(pid_t pid, void *addr)
  {
    _pid = pid;
    _addr = addr;
    ptrace(PTRACE_GETREGS, pid, NULL, &_oldRegs);
  }

  ~Breakpoints()
  {
  }

  void		putBreakpoint()
  {
    char		code[8];

    code[0] = 0xcc;
    injectCode(code, sizeof(code));
  }

  bool		injectCode(void *new_text, size_t len)
  {
    long	pokeData;

    if (len % sizeof(void *) != 0)
      {
	printf("invalid len, not a multiple of %zd\n", sizeof(void *));
	return false;
      }
    for (size_t copied = 0; copied < len; copied += sizeof(pokeData))
      {
	memmove(&pokeData, (new_text + copied), sizeof(pokeData));
	if (_oldText != NULL)
	  {
	    errno = 0;
	    long peek_data = ptrace(PTRACE_PEEKTEXT, _pid, (_addr + copied), NULL);
	    if (peek_data == -1 && errno)
	      {
		std::cerr << "Errno: " << strerror(errno) << std::endl;
		return false;
	      }
	    memmove(_oldText + copied, &peek_data, sizeof(peek_data));
	  }
	if (ptrace(PTRACE_POKETEXT, _pid, (_addr + copied), (void *)pokeData) < 0)
	  {
	    std::cerr << "Errno: " << strerror(errno) << std::endl;
	    return false;
	  }
      }
    return true;
  }

  void		resetProgram()
  {
    injectCode(_oldText, sizeof(_oldText));
    // ptrace(PTRACE_SETREGS, _pid, NULL, &_oldRegs);
  }
};
