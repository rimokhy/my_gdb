##
## Makefile for  in /home/rimokh_y/tracker
## 
## Made by Yohan Rimokh
## Login   <rimokh_y@epitech.net>
## 
## Started on  Wed May 18 02:54:39 2016 Yohan Rimokh
## Last update Wed May 18 03:08:28 2016 Yohan Rimokh
##

RM	= rm -rf

SRC	= src/Instruction.cpp \
	  src/main.cpp \
	  src/Symbols.cpp \
	  src/Syscall.cpp \
	  src/Tracer.cpp \

OBJ	= $(SRC:.cpp=.o)

CXXFLAGS	+= -I./include

CXXFLAGS	+= -Wall -Wextra

CC	= g++

NAME	= tracker

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(CXXFLAG) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

