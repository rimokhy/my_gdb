//
// RegsInteract.hpp for  in /home/rimokh_y/tracker/include
//
// Made by Yohan Rimokh
// Login   <rimokh_y@epitech.net>
//
// Started on  Thu May 26 00:01:32 2016 Yohan Rimokh
// Last update Thu May 26 02:29:02 2016 Yohan Rimokh
//

#pragma once

#include "Info.hh"

enum Type
  {
    Int,
    String,
    Char,
    Addr,
    Hexa
  };

class		RegsInteract
{
private:
  Registers	_regs;
public:
  typedef unsigned long long int	(RegsInteract::*GetRegs)(unsigned long long int value,
								 bool isPrint);
  typedef std::map<std::string, GetRegs>	Map;

  Map					_user;

  RegsInteract(Registers &regs):
    _regs(regs)
  {
    _user.insert(std::make_pair("r15", &RegsInteract::getR15));
    _user.insert(std::make_pair("r14", &RegsInteract::getR14));
    _user.insert(std::make_pair("r13", &RegsInteract::getR13));
    _user.insert(std::make_pair("r12", &RegsInteract::getR12));
    _user.insert(std::make_pair("r11", &RegsInteract::getR11));
    _user.insert(std::make_pair("r10", &RegsInteract::getR10));
    _user.insert(std::make_pair("r9", &RegsInteract::getR9));
    _user.insert(std::make_pair("r8", &RegsInteract::getR8));
    _user.insert(std::make_pair("rax", &RegsInteract::getRAX));
    _user.insert(std::make_pair("rcx", &RegsInteract::getRCX));
    _user.insert(std::make_pair("rdx", &RegsInteract::getRDX));
    _user.insert(std::make_pair("rsi", &RegsInteract::getRSI));
    _user.insert(std::make_pair("rdi", &RegsInteract::getRDI));
    _user.insert(std::make_pair("orig_rax", &RegsInteract::getORIGRAX));
    _user.insert(std::make_pair("rip", &RegsInteract::getRIP));
    _user.insert(std::make_pair("rsp", &RegsInteract::getRSP));
  }

  void				printRegister(std::string const &which)
  {
    if (_user.find(which) == _user.end())
      {
	std::cerr << "Cannot print " << which << std::endl;
	return;
      }
    std::cout << "Register " << which << ": " <<
      ((this->*_user[which])(0, true)) << std::endl;
  }

  void			        setRegister(std::string const &which,
					    unsigned long long int value)
  {
    if (_user.find(which) == _user.end())
      {
	std::cerr << "Cannot set " << which << std::endl;
	return;
      }
    ((this->*_user[which])(value, false));
  }

  unsigned long long int	getR15(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r15);
    else
      {
	_regs.r15 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r15);
     }
    return (0);
  }

  unsigned long long int	getR14(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r14);
    else
      {
	_regs.r14 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r14);
      }
    return (0);
  }

  unsigned long long int	getR13(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r13);
    else
      {
	_regs.r13 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r13);
      }
    return (0);
  }

  unsigned long long int	getR12(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r12);
    else
      {
	_regs.r12 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r12);
      }
    return (0);
  }

  unsigned long long int	getR11(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r11);
    else
      {
	_regs.r11 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r15);
      }
    return (0);
  }

  unsigned long long int	getR10(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r10);
    else
      {
	_regs.r10 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r10);
      }
    return (0);
  }

  unsigned long long int	getR9(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r9);
    else
      {
	_regs.r9 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r9);
      }
    return (0);
  }

  unsigned long long int	getR8(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.r8);
    else
      {
	_regs.r8 = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.r8);
      }
    return (0);
  }

  unsigned long long int	getRAX(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.rax);
    else
      {
	_regs.rax = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  {
	    std::cout << "Rax error:/\n";
	    return (0);
	  }
	return (_regs.rax);
      }
    return (0);
  }

  unsigned long long int	getRCX(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.rcx);
    else
      {
	_regs.rcx = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.rcx);
      }
    return (0);
  }

  unsigned long long int	getRDX(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.rdx);
    else
      {
	_regs.rdx = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.rdx);
      }
    return (0);
  }

  unsigned long long int	getRSI(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.rsi);
    else
      {
	_regs.rsi = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.rsi);
      }
    return (0);
  }

  unsigned long long int	getRDI(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.rdi);
    else
      {
	_regs.rdi = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.rdi);
      }
    return (0);
  }

  unsigned long long int	getORIGRAX(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.orig_rax);
    else
      {
	_regs.orig_rax = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.orig_rax);
      }
    return (0);
  }

  unsigned long long int	getRIP(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.rip);
    else
      {
	_regs.rip = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.rip);
      }
    return (0);
  }

  unsigned long long int	getRSP(unsigned long long int value, bool isPrint)
  {
    if (isPrint == true)
      return (_regs.rsp);
    else
      {
	_regs.rsp = value;
	if ((ptrace(PTRACE_SETREGS, g_pid, NULL, &_regs)) == -1)
	  return (0);
	return (_regs.rsp);
      }
    return (0);
  }

  ~RegsInteract()
  {
  }
};
