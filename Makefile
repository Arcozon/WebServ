NAME =  webserv

S_WEBSERV	=  WebServ.cpp  WebServInit.cpp
D_WEBSERV	=  webserv/
SRC_WEBSERV = $(addprefix $(D_WEBSERV), $(S_WEBSERV))

SRC   = $(SRC_WEBSERV)  main.cpp  MyException.cpp
D_SRC = src/

D_BUILD = .build/
OBJ =  $(addprefix $(D_BUILD), $(SRC:.cpp=.o))

D_INC = inc/  
INC = $(D_INC)

CPP =  c++
FLAGS = -Wall -Wextra -Werror -MMD -g -std=c++98
IFLAGS = $(addprefix "-I", $(INC)) 
RM =  rm -rf

MAKE += --no-print-directory
.DEFAULT_GOAL := test

test:
	@clear && $(MAKE) all && clear && ./$(NAME) test.config

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CPP) -o$@ $^

$(OBJ): $(D_BUILD)%.o:	$(D_SRC)%.cpp
	@mkdir -p $(@D)
	$(CPP) $(FLAGS) $(IFLAGS) -c $< -o $@ 

clean:
	$(RM) $(D_BUILD)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

DEPS = $(OBJ:.o=.d)
-include $(DEPS)

.PHONY: re fclean clean all

