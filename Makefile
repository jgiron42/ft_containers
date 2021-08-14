SRCS = 	 main.cpp 
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g
.cpp.o:
	clang++ $(CFLAGS) -c $< -o $(<:.cpp=.o)
NAME =  ft_containers 
all: $(NAME)
$(NAME): $(OBJS)
	clang++ $(CFLAGS) -o $(NAME) $(OBJS)
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re: fclean $(NAME)
.PHONY: clean fclean re
