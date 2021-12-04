SRCSFT =  mainft.cpp
OBJSFT = ${SRCSFT:.cpp=.o}

SRCSSTD =  mainstd.cpp
OBJSSTD = ${SRCSSTD:.cpp=.o}

CFLAGS = -std=c++98 -g3 -fsanitize=address -I.
.cpp.o:
	clang++ $(CFLAGS) -c $< -o $(<:.cpp=.o)
NAME =  containers

all: $(NAME)

$(NAME): ft_$(NAME) std_$(NAME)

std_$(NAME): $(OBJSSTD)
	clang++ $(CFLAGS) -o std_$(NAME) $(OBJSSTD)

ft_$(NAME): $(OBJSFT)
	clang++ $(CFLAGS) -o ft_$(NAME) $(OBJSFT)

compare: $(NAME)
	rm .fifo1 .fifo2; mkfifo .fifo1 .fifo2 ; ./std_$(NAME) map >> .fifo1 | ./ft_$(NAME) map >> .fifo2 | diff --color  .fifo1 .fifo2

clean:
	rm -f $(OBJSFT) $(OBJSSTD)

fclean: clean
	rm -f ft_$(NAME) std_$(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re all
