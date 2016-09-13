//
// Tracer.cpp for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Tue May  3 00:12:57 2016 Yohan RIMOKH
// Last update Thu May 26 02:38:24 2016 Yohan Rimokh
//

#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <algorithm>
#include "Exception.hpp"
#include "Tracer.hh"
#include "Instruction.hh"
#include "RegsInteract.hpp"
#include "Breakpoints.hpp"

int     g_pid;

Tracer::Tracer(char **av):
  _av(av),
  _process(NULL),
  _isBreakpointed(false)
{
}

Tracer::~Tracer()
{
}

void  Tracer::launch()
{
  try {
    _process = new Fork;
    if (_process->isSon())
      launchChild();
    else
      traceChild();
  } catch (Exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void  Tracer::launchChild()
{
  ptrace(PTRACE_TRACEME, 0, NULL, NULL);
  kill(getpid(), SIGSTOP);
  execvp(_av[1], &_av[1]);
}

void	Tracer::setBreakpoint(unsigned long brk)
{
  _breakpoints.push_back(brk);
}

bool	Tracer::isBreakPoint(unsigned long rip)
{
  for (int i = 0; i < _breakpoints.size(); i++)
    {
      if ((unsigned long long int)_breakpoints[i] == rip)
	{
	  std::cout << "Entering breakpoint " << i + 1
		    << " at " << rip << std::endl;
	  return (true);
	}
    }
  return (false);
}

bool  Tracer::traceChild()
{
  Instruction   instruction(_av[1]);
  Registers     regs;
  int           status;
  unsigned int	addrBrk;

  g_pid = _process->getPid();
  while (1)
   {
      _process->wait();
      status = _process->getStatus();
      if (_process->isSignal())
        break;
      if ((ptrace(PTRACE_GETREGS, _process->getPid(), NULL, &regs)) == -1)
        return (false);
      if (isBreakPoint(regs.rip))
      	{
      	  Breakpoints	brk(g_pid, (void*)regs.rip);

      	  char		code[8];
      	  code[0] = 0xcc;
      	  brk.injectCode(code, sizeof(code));
      	  std::string line;
      	  RegsInteract	interact(regs);
      	  while ((std::getline(std::cin, line)))
      	    {
	      std::string	command;
	      std::string	arg;

	      if ((line.find(" ")) != std::string::npos)
		{
		  command = line.substr(0, line.find(" "));
		  arg = line.substr(line.find(" ") + 1);
		  arg.erase(remove_if(arg.begin(), arg.end(), isspace), arg.end());
		}
	      else
		command = line;
	      if (command.empty())
		continue;
	      if (!command.compare("continue"))
      	  	break;
      	      else if (!command.compare("print"))
      	      	{
      	      	  std::string	arg = line.substr(line.find(" ") + 1);

      	      	  interact.printRegister(arg);
      	      	}
	      else if (!command.compare("set"))
	      	{
		  if (!arg.find("=") == std::string::npos)
		    {
		      std::cerr << "incorr syntax\n";
		      continue;
		    }
		  std::string	reg = arg.substr(0, arg.find("="));
		  std::string	val = arg.substr(arg.find("=") + 1);
	      	  unsigned long long int value = std::atoi(val.c_str());

	      	  interact.setRegister(reg, value);
	      	}
      	    }
      	  brk.resetProgram();
      	}
      instruction.executeInstruction(regs);
      if ((ptrace(PTRACE_SINGLESTEP, _process->getPid(), NULL, 0)) == -1)
        return (true);

    }
  return (true);
}
