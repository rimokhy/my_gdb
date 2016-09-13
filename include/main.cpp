//
// main.cpp for  in /home/rimokh_y/secu_training/tracker
// 
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
// 
// Started on  Wed May  4 05:30:30 2016 Yohan RIMOKH
// Last update Wed May  4 08:09:22 2016 Yohan RIMOKH
//
#include <iostream>
#include "Architecture.hpp"

int     main(int ac, char **av)
{
  if (av[1])
    {
      try {
        Archi archi(av[1]);

        archi.setArchi();
      }
      catch (Exception &e)
        {
          std::cerr << e.what() << std::endl;
          return (-1);
        }
    }
  else
    {
      std::cerr << "Set architecture for Traced filed" << std::endl;
      return (-1);
    }
  return (0);
}
