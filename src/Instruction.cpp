//
// Instruction.cpp for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Tue May  3 00:42:28 2016 Yohan RIMOKH
// Last update Sun May 22 07:10:43 2016 Yohan Rimokh
//

#include <sys/ptrace.h>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include "Instruction.hh"

static int     rax = -1;
static int      opcode = -1;

Instruction::Instruction( std::string const &filename)
{
  _map.insert(std::make_pair(Syscall, &Instruction::printSyscall));
  _map.insert(std::make_pair(Call, &Instruction::printCall));
  _map.insert(std::make_pair(Ret, &Instruction::printRet));
  _map.insert(std::make_pair(SysRet, &Instruction::printSysRet));
  _syscalls = new Syscalls;
  _isBreakpointed = false;
}

void          Instruction::printSysRet(Registers &regs)
{
  if (rax != -1)
    {
      if ((int)RETURN < 0)
        RETURN++;
      (*_syscalls)[rax].printReturnValue(regs);
    }
  rax = -1;
}

void    Instruction::printSyscall(Registers &regs)
{
  // (*_syscalls)[RETURN].printSyscall(regs);
  // rax = RETURN;
}

void    Instruction::printCall(Registers &regs)
{
  char  *str = new char[5];
  str[0] = 'b'; str[1] = 'i'; str[2] = 't'; str[3] = 'e'; str[4] = '\0';
  int   offset;
  Funct frame;
  offset = ptrace(PTRACE_PEEKTEXT, g_pid, CALL_PROC + 1, NULL) + 5;
  frame.name = ((*_symbols)[CALL_PROC + offset]).empty() ? "undefined" : (*_symbols)[CALL_PROC + offset];
  frame.addr = CALL_PROC + offset;
  _stack.push(frame);
  std::cout << "Entering function " << frame.name << "(";
  if (frame.name.find("cmp") != std::string::npos)
    {
      Prototype proto;
      proto.printString(PARAM1);
      std::cout << ", ";
      proto.printString(PARAM2);
    }
  std::cout << ") at " << std::hex << frame.addr << std::endl;

}

void          Instruction::printRegister(Registers &regs)
{
  // std::cout << "regs.rax" << regs.rax << std::endl;
  if (regs.rdi != 0)
    std::cout << "regs.rdi" << regs.rdi << std::endl;
  else if (regs.rsi != 0)
    std::cout << "regs.rsi" << regs.rsi << std::endl;
}

bool          Instruction::setRegister(Registers &regs)
{
  regs.rax = 0;
  if (ptrace(PTRACE_SETREGS, g_pid, &regs, &regs) == -1)
    return false;
  return true;
}

void    Instruction::printRet(Registers &regs)
{
  if (_stack.empty())
      return;
  std::cout << "Leaving function " << _stack.top().name << "\t = 0x" <<
    std::hex << RETURN << std::endl;
  if (_stack.top().name == "bidule")
    {
      // char      *backup;
      // _isBreakpointed = true;
      // std::cout << "YOPYOPYOPYOPYOP\n";
      //  RETURN = 0;
      // ptrace(PTRACE_SETREGS, g_pid, &regs, &regs);
    }
  _stack.pop();
}

bool          Instruction::isSyscall(Registers &regs)
{
  unsigned short	rip;

  rip = ptrace(PTRACE_PEEKTEXT, g_pid, CALL_PROC, 0);
  if (rip == 0xFFFF)
    return (false);
  if (rip == 0x80CD || rip == 0x50F)
      return (true);
  return (false);
}

bool          Instruction::isCall(Registers &regs)
{
  (void)regs;
  if (opcode == 0xE8)
    return (true);
  return (false);
}

bool          Instruction::isRetCall(Registers &regs)
{
  (void)regs;
  if (opcode == 0xC3)
    return (true);
  return (false);
}


bool    Instruction::executeInstruction(Registers &regs)
{
  InstructionPrinter printer = NULL;
  static bool        first = 1;
  uint32_t              instruction;
  int                   op;
  if (isSyscall(regs) || !first)
    {
      if (first)
        printer = _map[Syscall];
      else
        printer = _map[SysRet];
      first = !first;
    }
  else
    {
      if (CALL_PROC < UPPER_BOUND)
        {
          instruction = ptrace(PTRACE_PEEKTEXT, g_pid, CALL_PROC, NULL);
          op = instruction & 0xFF;
          opcode = op;
        }
      if (isCall(regs))
        {
          printer = _map[Call];
        }
      else if (isRetCall(regs))
        printer = _map[Ret];
      else if (op == 0xcc)
	std::cout << "Breakpoint\n";
      opcode = -1;
    }
  if (printer != NULL)
    (this->*printer)(regs);
  return true;
}

Instruction::~Instruction()
{
}
