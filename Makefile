SRCS = 	 main.cpp 
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -std=c++98 -g3 -fsanitize=memory
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
