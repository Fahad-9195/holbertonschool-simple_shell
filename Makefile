CC      := gcc
CFLAGS  := -Wall -Werror -Wextra -pedantic -std=gnu89
NAME    := hsh
SRC     := main.c tokenizer.c path.c errors.c builtins.c
OBJ     := $(SRC:.c=.o)

.PHONY: all clean fclean re betty

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

betty:
	@files="$(wildcard *.c) $(wildcard *.h)"; \
	if [ -z "$$files" ]; then echo "No C/H files to check."; \
	else betty $$files; fi
