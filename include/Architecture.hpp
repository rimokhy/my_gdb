//
// Architecture.hpp for  in /home/rimokh_y/secu_training/tracker
// 
// Made by Yohan RIMOKH
// Login   <rimokh_y@epitech.eu>
// 
// Started on  Wed May  4 06:17:56 2016 Yohan RIMOKH
// Last update Wed May  4 07:38:34 2016 Yohan RIMOKH
//

#pragma once

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <cstdlib>
#include <string.h>
#include "Exception.hpp"
#include <elf.h>

class           Archi
{
private:

  void          *data;
  Elf64_Ehdr    *elf;
public:
  int     filesize(int fd)
  {
    return (lseek(fd, 0, SEEK_END));
  }

  Archi(const char *file)
  {
    int           fd;

    if ((fd = open(file, O_RDONLY)) == -1)
      throw Exception("Open()");
    if ((data = mmap(NULL, filesize(fd), PROT_READ,
                        MAP_SHARED, fd, 0)) == (void*)-1)
      throw Exception("Mmap()");
    close(fd);
    if (data)
      elf = (Elf64_Ehdr*)data;
  }

  void                  setArchi()
  {
    bool                isFound = false;
    switch (elf->e_machine)
      {
      case EM_386:
        isFound = true;
        if ((system("cat include/Executioni386.hh > ./include/Info.hh")) < 0)
          throw Exception(strerror(errno));
        break;
      case EM_X86_64:
        isFound = true;
        if ((system("cat include/Execution64.hh > ./include/Info.hh")) < 0)
          throw Exception(strerror(errno));
        break;
      }
    if (!isFound)
      throw Exception("File architecture not handled");
  }
};
