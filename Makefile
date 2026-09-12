NAME	= select_practice
CXX	= cc
SRCS	= main.c
CFLAGS	= -Wall -Werror -Wextra

all:
	$(CXX) $(CFLAGS) -o $(NAME) $(SRCS)

run: all
	./$(NAME) 9999

.PHONY: all
