//
// Syscall.cpp for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Mon May  2 20:48:09 2016 Yohan RIMOKH
// Last update Fri May 20 21:06:29 2016 Yohan Rimokh
//

#include <cstring>
#include <cstdlib>
#include <sys/ptrace.h>
#include <stdio.h>
#include <errno.h>
#include "Syscall.hh"

Prototype::Prototype(int value, std::string const &returnType,
                     std::string const &name, std::vector<std::string> const &args):
  _value(value),
  _returnType(returnType),
  _name(name),
  _args(args)
{
  _map.insert(std::make_pair("int", &Prototype::printDecimal));
  _map.insert(std::make_pair("unsigned int", &Prototype::printDecimal));
  _map.insert(std::make_pair("unsigned long", &Prototype::printDecimal));
  _map.insert(std::make_pair("long", &Prototype::printDecimal));
  _map.insert(std::make_pair("char", &Prototype::printChar));
  _map.insert(std::make_pair("char *", &Prototype::printString));
  _map.insert(std::make_pair("void *", &Prototype::printAddr));
  _map.insert(std::make_pair("hexa", &Prototype::printHexa));
}

Prototype::~Prototype()
{
}

void    Prototype::printHexa(unsigned long value)
{
  std::cout << std::hex << "0x" << value;
}

void    Prototype::printDecimal(unsigned long value)
{
  std::cout << std::dec << value;
}

void    Prototype::printChar(unsigned long value)
{
  char  c = value;
  std::cout << c;
}

void    Prototype::printString(unsigned long value)
{
  char  *val = new char[1024];
  size_t        read = 0;
  unsigned long tmp = 0;
  bool          isFull = false;

  std::cout << "\"";
  if (!value)
    {
      std::cout << "NULL";
    }
  while(!isFull)
    {
      tmp = ptrace(PTRACE_PEEKDATA, g_pid, value + read);
      memcpy(val + read, &tmp, sizeof(tmp));
      if (memchr(&tmp, 0, sizeof(tmp)) != NULL)
        isFull = true;
      read += sizeof(tmp);
    }
  std::string real(val);
  for (size_t i = 0; i < real.size(); i++)
    if (real[i] == '\n')
      std::cout << "\\n";
    else
      {
        if (real[i] >= 32 && real[i] <= 126)
          std::cout << real[i];
        else
          std::cout << std::hex << real[i];
      }
  delete[] val;
  std::cout << "\"";
}

void    Prototype::printAddr(unsigned long value)
{
  if (value == 0)
    printf("NULL");
  else
    printf("0x%p", (void*)&value);
}

int   Prototype::getValue() const
{
  return _value;
}

std::string   Prototype::getName() const
{
  return _name;
}

std::string   Prototype::getReturnType() const
{
  return _returnType;
}

size_t           Prototype::getArgsNumber() const
{
  return _args.size();
}

std::vector<std::string>        Prototype::getArgs() const
{
  return _args;
}

void          Prototype::printSyscall(Registers &regs)
{
  if (!_name.compare("read"))
    return;
  std::cout << getName() << "(";
  if (getArgsNumber() > 0)
    {
      for (size_t i = 0; i < _args.size(); i++)
        {
          argsPrinter   printer = NULL;
          unsigned long  param;
          if (_map.find(_args[i]) == _map.end())
            printer = _map["hexa"];
          else
            printer = _map[_args[i]];
          switch (i)
            {
            case 0:
              param = PARAM1;
              break;
            case 1:
              param = PARAM2;
              break;
            case 2:
              param = PARAM3;
              break;
            case 3:
              param = PARAM4;
              break;
            case 4:
              param = PARAM5;
              break;
            case 5:
              param = PARAM6;
              break;
            }
          (this->*printer)(param);
           if (i + 1 != _args.size())
            std::cout << ", ";
        }
    }
  std::cout << ")";
  if (!_returnType.compare("void"))
    std::cout << "\t= ?" << std::endl;
}

void          Prototype::printReturnValue(Registers &regs)
{
  if (!_returnType.compare("void"))
    return;
  else if (!_name.compare("read"))
    {
      std::cout << getName() << "(";
      printDecimal(regs.rdi);
      std::cout << ", \"";
      printString(regs.rsi);
      std::cout << "\", ";
      printDecimal(regs.rdx);
      std::cout << ")";
      std::cout << std::hex << "\t= 0x" << RETURN << std::endl;
    }
  else
    std::cout << std::hex << "\t= 0x" << RETURN << std::endl;
}

void            Prototype::dump()
{
  std::cout << "[" << _value << "]: " << _returnType << " " << _name << "(";
  for (size_t i = 0; i < _args.size(); i++)
    std::cout << _args[i] << (i + 1 == _args.size() ? "" : ", ");
  std::cout << ");\n";
}

Syscalls::Syscalls( std::string const &filename):
  _filename(filename)
{
  getSyscalls();
}

unsigned int    Syscalls::getValue(std::string const &line, int *i)
{
  std::string   value;
  int           val;

  while (line[*i] && isdigit(line[*i]))
    {
      value += line[*i];
      (*i) += 1;
    }
  val = atoi(value.c_str());
  while (line[*i] && isblank(line[*i]))
    *i += 1;
  return (val);
}

std::string     Syscalls::getReturnType(std::string const &line, int *i)
{
  std::string   name;

  while (line[*i] && !isblank(line[*i]))
    {
      name += line[*i];
      (*i) += 1;
    }
  if (name == "unsigned")
    {
      name += ' ';
      while (line[*i] && isblank(line[*i]))
        *i += 1;
      while (line[*i] && !isblank(line[*i]))
        {
          name += line[*i];
          (*i) += 1;
        }
    }
  while (line[*i] && isblank(line[*i]))
    (*i) += 1;
  return (name);
}

std::string     Syscalls::getName(std::string const& line, int *i)
{
  std::string   name;

  while (line[*i] && !isblank(line[*i]))
    {
      name += line[*i];
      (*i) += 1;
    }
  while (line[*i] && isblank(line[*i]))
    (*i) += 1;
  return (name);
}

void                            endSpaces(std::string &line)
{
  const size_t strBegin = line.find_first_not_of(" \t\n");
  if (strBegin == std::string::npos)
    return ;
  const size_t strEnd = line.find_last_not_of(" \t\n");
  const size_t strRange = strEnd - strBegin + 1;
  line = line.substr(strBegin, strRange);
}

std::vector<std::string>        Syscalls::getArgs(char *line)
{
  char                          *tok;
  std::vector<std::string>      args;
  std::string                   param;

  while ((tok = strtok(line, ",;")) != NULL)
    {
      param = tok;
      endSpaces(param);
      args.push_back(param);
      line = NULL;
    }
  return (args);
}

bool            Syscalls::getSyscalls()
{
  std::ifstream stream(_filename.c_str());

  if (stream.is_open())
    {
      std::string       line;
      int               i;
      while (getline(stream, line))
        {
          i = 0;
          int           value  = getValue(line, &i);
          std::string   retType = getReturnType(line, &i);
          std::string   name = getName(line, &i);
          Prototype             prototype(value, retType, name, getArgs(&line[i]));

          _syscalls.push_back(prototype);
        }
      stream.close();
    }
  return (false);
}

Prototype     Syscalls::operator[](unsigned int key)
{
  return _syscalls[key];
}

Syscalls::~Syscalls()
{
}
