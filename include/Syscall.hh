//
// Syscall.hh for  in /home/rimokh_y/secu_training/tracker
// 
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
// 
// Started on  Mon May  2 20:48:02 2016 Yohan RIMOKH
// Last update Wed May  4 14:00:34 2016 Yohan RIMOKH
//

#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "Info.hh"

class   Prototype
{
public:
  typedef void (Prototype::*argsPrinter)(unsigned long value);
  typedef std::map<std::string, argsPrinter> ArgsMapping;
  Prototype(){}
  Prototype(int value, std::string const &returnType,
            std::string const &name, std::vector<std::string> const &args);
  ~Prototype();
  void  printDecimal(unsigned long value);
  void  printHexa(unsigned long value);
  void  printChar(unsigned long value);
  void  printString(unsigned long value);
  void  printAddr(unsigned long value);
  int   getValue() const;
  std::string   getName() const;
  std::string   getReturnType() const;
  size_t           getArgsNumber() const;
  std::vector<std::string>        getArgs() const;
  void                          dump();
  void          printSyscall(Registers &regs);
  void          printReturnValue(Registers &regs);
private:
  int   _value;
  std::string   _returnType;
  std::string   _name;
  std::vector<std::string>      _args;
  ArgsMapping                   _map;
};

class   Syscalls
{
private:
  std::string   _filename;
  std::vector<Prototype>        _syscalls;
public:
  Syscalls(std::string const &filename = DB_SYSC);
  ~Syscalls();
  bool            getSyscalls();
  std::vector<std::string>        getArgs(char *line);
  std::string     getName(std::string const& line, int *i);
  std::string     getReturnType(std::string const &line, int *i);
  unsigned int    getValue(std::string const &line, int *i);
  Prototype     operator[](unsigned int key);
};
