//
// Tracer.hh for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Mon May  2 20:47:14 2016 Yohan RIMOKH
// Last update Thu May 26 01:07:27 2016 Yohan Rimokh
//

#pragma once

#include <string>
#include <sys/ptrace.h>
#include <csignal>
#include <vector>
#include "Fork.hpp"

class   Tracer
{
  char          **_av;
  Fork          *_process;
  pid_t		_pid;
  std::vector<unsigned long> _breakpoints;
  bool		_isBreakpointed;
public:
  Tracer(char **av);
  ~Tracer();
  void	setBreakpoint(unsigned long);
  bool	isBreakPoint(unsigned long rip);
  void  launch();
  void  launchChild();
  bool  traceChild();
};
