NAME =  webserv

S_CGIHANDLER	=  CGI.cpp  CGI_Child.cpp  CGI_Env.cpp
D_CGIHANDLER	=  CGIHandler/
SRC_CGIHANDLER	=  $(addprefix $(D_CGIHANDLER), $(S_CGIHANDLER))

S_CONFIG	=  IpPort.cpp  Location.cpp  Return.cpp
D_CONFIG	=  Config/
SRC_CONFIG	=  $(addprefix $(D_CONFIG), $(S_CONFIG))

S_SERVER   =  Server.cpp Client.cpp
S_SERVER  +=  Cookies.cpp Sessions.cpp
D_SERVER   =  Server/
SRC_SERVER =  $(addprefix $(D_SERVER), $(S_SERVER))

S_RESPONSE   =  Response.cpp  ResponseGet.cpp  ResponsePost.cpp  ResponseDelete.cpp
S_RESPONSE  +=  ResponseAutoIndex.cpp  ResponseError.cpp  ResponseReturn.cpp  ResponseCGI.cpp
D_RESPONSE	 =  Response/
SRC_RESPONSE =  $(addprefix $(D_RESPONSE), $(S_RESPONSE))

S_PARS	=  ParsLine.cpp  ParsConfig.cpp  ParsLocation.cpp  ParsIpPort.cpp
D_PARS	=  Parsing/
SRC_PARS =  $(addprefix $(D_PARS), $(S_PARS))

S_UTILS		=  FStat.cpp  MakeHTML.cpp  MyException.cpp  ReadDir.cpp  $(SRC_PARS)
D_UTILS		=  Utils/
SRC_UTILS	=  $(addprefix $(D_UTILS), $(S_UTILS))

SRC   =  $(SRC_UTILS)  $(SRC_SERVER)  $(SRC_RESPONSE)  $(SRC_CONFIG)  $(SRC_CGIHANDLER) 
SRC  +=  main.cpp
D_SRC =  src/

D_BUILD =  .build/
OBJ		=  $(addprefix $(D_BUILD), $(SRC:.cpp=.o))

D_INC =  inc/
S_INC =  . Parsing  CGI  Config utils
INC	  =  $(addprefix $(D_INC), $(S_INC))

CPP		=  c++
FLAGS	=  -Wall -Wextra -Werror -MMD -g3 -std=c++98
IFLAGS	=  $(addprefix "-I", $(INC)) 

RM =  rm -rf

VAL = valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all -q
MAKE += --no-print-directory
.DEFAULT_GOAL := all

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

