NAME =  webserv

S_PARSLOCATION	 =  ParsLocation.cpp
D_PARSLOCATION	 =  ParsLocation/
SRC_PARSLOCATION =  $(addprefix $(D_PARSLOCATION), $(S_PARSLOCATION))

S_LOCATION	 =  $(SRC_PARSLOCATION)  Location.cpp
D_LOCATION	 =  Location/
SRC_LOCATION =  $(addprefix $(D_LOCATION), $(S_LOCATION))

S_PARSSERVER   =  ParsServer.cpp
D_PARSSERVER   =  ParsServer/
SRC_PARSSERVER =  $(addprefix $(D_PARSSERVER), $(S_PARSSERVER))

S_SERVER   =  $(SRC_PARSSERVER)  Server.cpp
D_SERVER   =  Server/
SRC_SERVER =  $(addprefix $(D_SERVER), $(S_SERVER))

S_PARSWEBSERV	=  init.cpp  ParsWebServ.cpp
D_PARSWEBSERV	=  ParsWebServ/
SRC_PARSWEBSERV =  $(addprefix $(D_PARSWEBSERV), $(S_PARSWEBSERV))

S_WEBSERV	=  $(SRC_PARSWEBSERV)  WebServ.cpp
D_WEBSERV	=  WebServ/
SRC_WEBSERV =  $(addprefix $(D_WEBSERV), $(S_WEBSERV))

SRC   =  $(SRC_WEBSERV)  $(SRC_SERVER)  $(SRC_LOCATION)
SRC  +=  main.cpp  MyException.cpp
D_SRC =  src/

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

