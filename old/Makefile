CC = g++
# FLAGS = -std=c++98 
FLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
RM = rm -f

NAME = ircserv
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all = $(NAME)

$(NAME): $(OBJS) 
		$(CC) $(SRCS) -o $(NAME)

run: all
	./$(NAME)

clean:
	$(RM) $(OBJS) $(NAME)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
