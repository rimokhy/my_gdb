//
// Executioni386.hh for  in /home/rimokh_y/secu_training/tracker
// 
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
// 
// Started on  Wed May  4 05:41:21 2016 Yohan RIMOKH
// Last update Wed May  4 14:01:27 2016 Yohan RIMOKH
//

#pragma once

extern int      g_pid;

struct user_fpregs_struct
{
  long int cwd;
  long int swd;
  long int twd;
  long int fip;
  long int fcs;
  long int foo;
  long int fos;
  long int st_space [20];
};

struct user_fpxregs_struct
{
  unsigned short int cwd;
  unsigned short int swd;
  unsigned short int twd;
  unsigned short int fop;
  long int fip;
  long int fcs;
  long int foo;
  long int fos;
  long int mxcsr;
  long int reserved;
  long int st_space[32];   /* 8*16 bytes for each FP-reg = 128 bytes */
  long int xmm_space[32];  /* 8*16 bytes for each XMM-reg = 128 bytes */
  long int padding[56];
};

struct user_regs_struct
{
  long int ebx;
  long int ecx;
  long int edx;
  long int esi;
  long int edi;
  long int ebp;
  long int eax;
  long int xds;
  long int xes;
  long int xfs;
  long int xgs;
  long int orig_eax;
  long int eip;
  long int xcs;
  long int eflags;
  long int esp;
  long int xss;
};

struct user
{
  struct user_regs_struct	regs;
  int				u_fpvalid;
  struct user_fpregs_struct	i387;
  unsigned long int		u_tsize;
  unsigned long int		u_dsize;
  unsigned long int		u_ssize;
  unsigned long			start_code;
  unsigned long			start_stack;
  long int			signal;
  int				reserved;
  struct user_regs_struct*	u_ar0;
  struct user_fpregs_struct*	u_fpstate;
  unsigned long int		magic;
  char				u_comm [32];
  int				u_debugreg [8];
};

#define PAGE_SHIFT		12
#define PAGE_SIZE		(1UL << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))
#define NBPG			PAGE_SIZE
#define UPAGES			1
#define HOST_TEXT_START_ADDR	(u.start_code)
#define HOST_STACK_END_ADDR	(u.start_stack + u.u_ssize * NBPG)

typedef user_regs_struct Registers;

# define PARAM1 regs.ebx

# define PARAM2 regs.ecx

# define PARAM3 regs.edx

# define PARAM4 0

# define PARAM5 0

# define PARAM6 0

# define CALL_PROC regs.eip

# define UPPER_BOUND 0xb0000000

# define RETURN regs.eax

# define DB_SYSC "./syscalls_db/syscalls32.txt"
