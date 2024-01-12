NAME      = servIO
CC        = c++ -std=c++98 
FLAGS     = -Wall -Wextra -Werror
RM        = rm -fr
OBJDIR    = .obj
FILES     = Parser/Lexer \
		Parser/MainContext \
		Parser/Parser \
		Parser/Token \
		servIO utils
		# Parser/ServerContext \
SRC       = $(FILES:=.cpp)
OBJ       = $(addprefix $(OBJDIR)/, $(FILES:=.o))
INCLUEDES = includes/servIO.hpp \
		Parser/Lexer.hpp \
		Parser/MainContext.hpp \
		Parser/Parser.hpp \
		Parser/Token.hpp \
		# Parser/ServerContext.hpp \

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp $(INCLUEDES)
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@ -I ./includes/ -I ./Parser

clean:
	$(RM) $(OBJDIR) $(OBJ)

fclean: clean
	$(RM)  $(NAME)

re: fclean all
.PHONY: all clean fclean re
