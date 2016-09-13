//
// Exception.hpp for  in /home/rimokh_y/secu_training/tracker
// 
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
// 
// Started on  Tue May  3 00:21:49 2016 Yohan RIMOKH
// Last update Tue May  3 00:31:18 2016 Yohan RIMOKH
//

#pragma once

#include <exception>
#include <string>
class Exception: public std::exception
{
protected:
  std::string _msg;
public:
  explicit Exception(const std::string& message):
    _msg(message)
  {}
  virtual ~Exception() throw (){}
  virtual const char* what() const throw (){
    return _msg.c_str();
  }
};
