NAME =  

SRC   = main.cpp
D_SRC = src/

D_INC = inc/  

D_BUILD = .build/
OBJ =  $(addprefix $(D_BUILD), $(SRC:.cpp=.o))


CPP =  c++
FLAGS = -Wall -Wextra -Werror -MMD -g -std=c++98

RM =  rm -rf

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CPP) -o$@ $^

$(OBJ): $(D_BUILD)%.o:	$(D_SRC)%.cpp
	@mkdir -p $(@D)
	$(CPP) $(FLAGS) -I$(D_INC) -c $< -o $@ 

clean:
	$(RM) $(D_BUILD)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

DEPS = $(OBJ:.o=.d)
-include $(DEPS)

.PHONY: re fclean clean all

