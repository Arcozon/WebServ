NAME =  webserv

S_CGIHANDLER	=  CGI.cpp  CGI_Child.cpp  CGI_Env.cpp
D_CGIHANDLER	=  CGIHandler/
SRC_CGIHANDLER	=  $(addprefix $(D_CGIHANDLER), $(S_CGIHANDLER))

S_RETURN	=  Return.cpp
D_RETURN	=  Return/
SRC_RETURN	=  $(addprefix $(D_RETURN), $(S_RETURN))

S_PARSLOCATION	 =  ParsLocation.cpp
D_PARSLOCATION	 =  ParsLocation/
SRC_PARSLOCATION =  $(addprefix $(D_PARSLOCATION), $(S_PARSLOCATION))

S_LOCATION	 =  $(SRC_PARSLOCATION)  Location.cpp
D_LOCATION	 =  Location/
SRC_LOCATION =  $(addprefix $(D_LOCATION), $(S_LOCATION))

S_PARSIPPORT   =  ParsIpPort.cpp
D_PARSIPPORT   =  ParsIpPort/
SRC_PARSIPPORT =  $(addprefix $(D_PARSIPPORT), $(S_PARSIPPORT))

S_IPPORT   =  $(SRC_PARSIPPORT)  IpPort.cpp
D_IPPORT   =  IpPort/
SRC_IPPORT =  $(addprefix $(D_IPPORT), $(S_IPPORT))

S_SERVER   =  Server.cpp Client.cpp
S_SERVER  +=  Response.cpp  ResponseGet.cpp  ResponsePost.cpp  ResponseDelete.cpp
S_SERVER  +=  ResponseAutoIndex.cpp  ResponseError.cpp  ResponseReturn.cpp
D_SERVER   =  Server/
SRC_SERVER =  $(addprefix $(D_SERVER), $(S_SERVER))

S_PARSWEBSERV	=  ParsWebServ.cpp
D_PARSWEBSERV	=  ParsWebServ/
SRC_PARSWEBSERV =  $(addprefix $(D_PARSWEBSERV), $(S_PARSWEBSERV))

S_WEBSERV	=  $(SRC_PARSWEBSERV)  WebServ.cpp
D_WEBSERV	=  WebServ/
SRC_WEBSERV =  $(addprefix $(D_WEBSERV), $(S_WEBSERV))

SRC   =  $(SRC_WEBSERV)  $(SRC_SERVER)  $(SRC_IPPORT)  $(SRC_LOCATION)  $(SRC_RETURN)  $(SRC_CGIHANDLER)
SRC  +=  main.cpp  MyException.cpp  ParsLine.cpp  FStat.cpp  ReadDir.cpp  MakeHTML.cpp
D_SRC =  src/

D_BUILD =  .build/
OBJ		=  $(addprefix $(D_BUILD), $(SRC:.cpp=.o))

D_INC =  inc/
S_INC =  . Pars  CGI
INC	  =  $(addprefix $(D_INC), $(S_INC))

CPP		=  c++
FLAGS	=  -Wall -Wextra -Werror -MMD -g -std=c++98
IFLAGS	=  $(addprefix "-I", $(INC)) 

RM =  rm -rf

VAL = valgrind --leak-check=full --show-leak-kinds=all -q
MAKE += --no-print-directory
.DEFAULT_GOAL := test

test:
	@clear && $(MAKE) all && clear && echo '\t./$(NAME)' test.config && $(VAL) ./$(NAME) test.config
# curl -X POST localhost:8080 -H "Le G le : Eudes" -d "le eudes le g"

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CPP) -o$@ $^

$(OBJ): $(D_BUILD)%.o:	$(D_SRC)%.cpp
	@mkdir -p $(@D) $(@D)
	$(CPP) $(FLAGS) $(IFLAGS) -c $< -o $@ 

clean:
	$(RM) $(D_BUILD)

fclean: clean
	$(RM) $(NAME)

re: fclean
	$(MAKE) all

DEPS = $(OBJ:.o=.d)
-include $(DEPS)

.PHONY: re fclean clean all

