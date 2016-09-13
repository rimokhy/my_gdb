//
// main.cpp for  in /home/rimokh_y/secu_training/tracker
// 
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
// 
// Started on  Mon May  2 19:24:57 2016 Yohan RIMOKH
// Last update Sat May  7 21:33:04 2016 Yohan RIMOKH
//

#include "Interface.hpp"
#include "Exception.hpp"
#include "Tracer.hh"

int     main(int ac, char **av)
{
  (void)ac;
  if (av[1])
    {
      Interface interface(av[1]);

      return (interface.launchCmd());
      // Tracer            tracer(av);

      // try
      //   {
      //     tracer.launch();
      //   }
    }
  else
    std::cerr << "./tracer [prog]" << std::endl;
  return (0);
}
