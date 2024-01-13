NAME      = webserv
CC        = c++ -std=c++98
FLAGS     = -Wall -Wextra -Werror
RM        = rm -fr
OBJDIR    = .obj
FILES     = Core/MainContext Core/Poller Core/ServIO Core/Socket main parser/Lexer parser/Parser parser/Token utils
SRC       = $(FILES:=.cpp)
OBJ       = $(addprefix $(OBJDIR)/, $(FILES:=.o))
INCLUEDES = Core/MainContext.hpp Core/Poller.hpp Core/Socket.hpp includes/servIO.hpp parser/Lexer.hpp parser/Parser.hpp parser/Token.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp $(INCLUEDES)
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@ -I ./includes -I ./parser/ -I ./Core

clean:
	$(RM) $(OBJDIR) $(OBJ)

fclean: clean
	$(RM)  $(NAME)

re: fclean all
.PHONY: all clean fclean re

