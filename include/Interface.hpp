//
// Interface.hpp for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Sat May  7 20:46:58 2016 Yohan RIMOKH
// Last update Thu May 26 01:09:37 2016 Yohan Rimokh
//

#pragma once

#include <string>
#include "Tracer.hh"
#include <iostream>
#include <map>
#include <vector>
#include "Symbols.hh"

extern Symbols	*_symbols;

class   Interface
{
private:
  std::string   _exec;
  bool          _run;
  Tracer        *_tracer;
  std::vector<unsigned long>	_breakpoints;
  typedef void (Interface::*Cmd)(std::string const &);
  typedef std::map<std::string, Cmd> CmdMapping;
  CmdMapping    _map;
public:
  Interface(std::string const &exec): _exec(exec), _run(false)
  {
    _map.insert(std::make_pair("run", &Interface::runTracee));
    _map.insert(std::make_pair("switch", &Interface::runTracee));
    _map.insert(std::make_pair("exit", &Interface::exitCmd));
    _map.insert(std::make_pair("pid", &Interface::exitCmd));
    _map.insert(std::make_pair("disassemble", &Interface::disassemble));
    _map.insert(std::make_pair("brk", &Interface::setBreakpoint));
    _symbols = new Symbols(exec);
    // _map.insert(std::make_pair("print string", &Interface::setBreakpoint));
    // _map.insert(std::make_pair("print hex", &Interface::setBreakpoint));
    // _map.insert(std::make_pair("print dec", &Interface::setBreakpoint));
    // _map.insert(std::make_pair("print regs", &Interface::setBreakpoint));
  }

  bool          doAction(std::string const &line)
  {
    size_t      pos;

    if ((pos = line.find(" ")) != std::string::npos)
      pos = pos;
    else
      pos = line.length();
    if (_map.find(line.substr(0, pos)) != _map.end())
      {
        Cmd     cmd;
        cmd = _map[line.substr(0, pos)];
        (this->*cmd)(line.substr(pos));
        return true;
      }
    return false;
  }

  void          runTracee(std::string const &line)
  {
    std::vector<std::string> _params;
    size_t      lastIndex = 0;
    char        **av = NULL;

    _params.push_back("useless");
    _params.push_back(_exec);
    for (size_t i = 0; i < line.length(); i++)
      {
        if (line[i] == ' ' || line[i + 1] == '\0')
          {
            std::string arg = line.substr(lastIndex + 1, i - lastIndex);
            if (!arg.empty())
              _params.push_back(arg);
            lastIndex = i;
          }
      }
    av = new char*[_params.size() + 1];
    size_t i;
    for (i = 0; i < _params.size(); i++)
      av[i] = const_cast<char*>(_params[i].c_str());
    av[i] = NULL;
    _tracer = new Tracer(av);
    try {
      if (!_breakpoints.empty())
	{
	  for (size_t i = 0; i < _breakpoints.size(); i++)
	    _tracer->setBreakpoint(_breakpoints[i]);
	}
      _tracer->launch();
    } catch (Exception &e) {
      std::cout << e.what() << std::endl;
    }
    delete[] av;
  }

  void	        setBreakpoint(std::string const &line)
  {
    if (line.find("*") == std::string::npos)
      {
	std::cerr << "Usage\n";
	return ;
      }
    unsigned long	addr = std::strtoul((line.substr(line.find("*") + 1)).c_str(), NULL, 16);
    _breakpoints.push_back(addr);
    std::cout << std::hex << addr << std::endl;
  }

  void		disassemble(std::string const &line)
  {
    if (line.find(" ") == std::string::npos)
      {
	std::cerr << "Help\n";
	return;
      }
    std::string	name = line.substr(line.find(" ") + 1);
    std::vector<std::string>	disassemble = (*_symbols)[name];
    for (size_t i = 0; i < disassemble.size(); i++)
      std::cout << disassemble[i] << std::endl;
    std::cout << "END\n";
  }

  void          exitCmd(std::string const &line)
  {
    (void)line;
    delete _tracer;
    _run = false;
  }

  bool          launchCmd()
  {
    std::string       line;

    _run = true;
    while (_run && std::getline(std::cin, line))
      {
        if (!doAction(line) && !line.empty())
          std::cout << "Unknown command" << std::endl;
      }
    return true;
  }
};
