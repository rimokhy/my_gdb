//
// Instruction.hh for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Tue May  3 00:43:19 2016 Yohan RIMOKH
// Last update Fri May 20 06:47:58 2016 Yohan Rimokh
//

#pragma once

#include <map>
#include <iostream>
#include <stack>
#include "Symbols.hh"
#include "Syscall.hh"

extern Symbols	*_symbols;

class   Funct
{
public:
  std::string   name;
  unsigned long addr;
};

enum InstructionType
  {
    Syscall,
    SysRet,
    Call,
    Ret
  };

class   Instruction
{
public:
  typedef void (Instruction::*InstructionPrinter)(Registers &regs);
  typedef std::map<InstructionType, InstructionPrinter> FunctionMap;
  Instruction(std::string const&);
  ~Instruction();
  bool          isSyscall(Registers &regs);
  bool          isCall(Registers &regs);
  bool          isRetCall(Registers &regs);
  void          printSyscall(Registers &regs);
  void          printSysRet(Registers &regs);
  void          printCall(Registers &regs);
  void          printRet(Registers &regs);
  bool          executeInstruction(Registers &regs);
  void          printRegister(Registers &regs);
  bool          setRegister(Registers &regs);
private:
  FunctionMap           _map;
  int                   _pid;
  Syscalls       *_syscalls;
  std::stack<Funct>     _stack;
  bool          _isBreakpointed;
  long          _backup;
};
