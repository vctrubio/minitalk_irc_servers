
CC = c++
FLAGS = -std=c++98 
# FLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
RM = rm -f

NAME = irc
SRCS = $(wildcard ./srcs/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all = $(NAME)

$(NAME):
		$(CC) $(FLAGS) $(SRCS) -o $(NAME)

run: all
	./$(NAME)

clean:
		$(RM) $(OBJS)

fclean: clean
		$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
