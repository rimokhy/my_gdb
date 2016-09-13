//
// Fork.hpp for  in /home/rimokh_y/secu_training/tracker
//
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
//
// Started on  Mon May  2 21:51:25 2016 Yohan RIMOKH
// Last update Sun May 22 07:05:52 2016 Yohan Rimokh
//

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include "Exception.hpp"

class   Fork
{
private:
  pid_t _pid;
  int   _status;
public:
  Fork()
  {
    _pid = fork();
    if (_pid == -1)
      throw Exception("Fork failed.\n");
  }

  int   getStatus()
  {
    return _status;
  }

  bool  isSon()
  {
    return !_pid;
  }

  void  wait()
  {
    waitpid(_pid, &_status, WCONTINUED);
  }

  int   getPid()
  {
    return _pid;
  }

  bool  isSignal()
  {
    int		sig;

    sig = WSTOPSIG(_status);
    if (WIFEXITED(_status) || WIFSIGNALED(_status) || !WIFSTOPPED(_status))
      return true;
    return false;
  }

  ~Fork()
  {
  }
};
