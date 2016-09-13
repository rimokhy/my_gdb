//
// Symbols.cpp for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Mon May  2 19:25:24 2016 Yohan RIMOKH
// Last update Sun May 22 07:05:53 2016 Yohan Rimokh
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "Symbols.hh"

Symbols	*_symbols = NULL;

Function::Function(std::string name, std::vector<std::string> code)
{
  _name = name;
  _code = code;
}

Function::~Function()
{
}

std::string	Function::getName() const
{
  return _name;
}

std::vector<std::string>	Function::getCode() const
{
  return _code;
}

Symbols::Symbols(std::string const &filename):
  _filename(filename)
{
  initializeData();
}

unsigned long   Symbols::getAddr(std::string const &line)
{
  std::stringstream     ss;

  for (int i = 0; i < 16; i++)
    ss << line[i];
  return (ss.str().empty() ? 0 : std::strtoul(ss.str().c_str(), NULL, 16));
}

std::string   Symbols::getName(std::string const &line)
{
  size_t        posBegin;
  size_t        posEnd;

  if ((posBegin = line.find("<") + 1) != std::string::npos &&
      ((posEnd = line.find_last_of("@")) != std::string::npos ||
       (posEnd = line.find_last_of(">")) != std::string::npos))
    return (line.substr(posBegin, posEnd - posBegin));
  return (std::string(""));
}

bool    Symbols::isValidLine(std::string const &line)
{
  for (int i = 0; i < 16; i++)
    {
      if (!((line[i] >= 'a' && line[i] <= 'f') ||
            (line[i] >= 'A' && line[i] <= 'F') ||
            isdigit(line[i])))
        return (false);
    }
  if ((line.find("<") + 1) != std::string::npos &&
      ((line.find_last_of("@")) != std::string::npos ||
       (line.find_last_of(">")) != std::string::npos))
    return (true);
  return (false);
}

bool    Symbols::initializeData()
{
  std::stringstream        os;
  std::ifstream         reader;
  std::string           line;
  bool			next = false;

  os << "objdump -S " << _filename <<
    " > logs";
  std::system(os.str().c_str());
  reader.open(std::string("logs").c_str());
  if (reader.is_open())
    {
      std::vector<std::string> code;
      unsigned long addr;
      std::string   name;

      while (getline(reader, line))
        {
	  if (line.empty() && !name.empty())
	    {
	      _functmapping[addr] = new Function(name, code);
	      name.clear();
	      code.clear();
	    }
	  else if (isValidLine(line))
	    {
	      name = getName(line);
	      addr = getAddr(line);
	    }
	  else
	    {
	      code.push_back(line);
	    }
	}
      reader.close();
      return true;
    }
  return false;
}

std::string   Symbols::operator[](unsigned long found)
{
  for (map::iterator it = _functmapping.begin();
       it != _functmapping.end();
       ++it)
    if (it->first == found)
      {
        return (it->second->getName());
      }
  return (std::string(""));
}

std::vector<std::string>   Symbols::operator[](std::string const &found)
{
  for (map::iterator it = _functmapping.begin();
       it != _functmapping.end();
       ++it)
    if (it->second->getName() == found)
      {
        return (it->second->getCode());
      }
  return (std::vector<std::string>());
}

Symbols::~Symbols()
{
}
