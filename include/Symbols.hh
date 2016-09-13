//
// Symbols.hh for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Mon May  2 19:25:33 2016 Yohan RIMOKH
// Last update Fri May 20 06:52:58 2016 Yohan Rimokh
//

#pragma once

#include <string>
#include <map>
#include <vector>
#include <cstdlib>

class   Function
{
private:
  std::string	_name;
  std::vector<std::string>	_code;
public:
  Function(std::string name, std::vector<std::string> code);
  ~Function();
  std::string	getName() const;
  std::vector<std::string>	getCode() const;
};

typedef std::map<unsigned long, Function*> map;

class   Symbols
{
private:
  std::string   _filename;
  map           _functmapping;
public:
  Symbols(std::string const &);
  ~Symbols();
  bool          initializeData();
  bool          isValidLine(std::string const &line);
  unsigned long getAddr(std::string const &line);
  std::string   getName(std::string const &line);
  void          dump();
  std::string   operator[](unsigned long found);
  std::vector<std::string>   operator[](std::string const &name);
};
