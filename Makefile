CC      := gcc
CFLAGS  := -Wall -Werror -Wextra -pedantic -std=gnu89
NAME    := hsh
SRC     := main.c

.PHONY: all clean fclean re betty

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

betty:
	@files="$(wildcard *.c) $(wildcard *.h)"; \
	if [ -z "$$files" ]; then echo "No C/H files to check."; else betty $$files; fi
