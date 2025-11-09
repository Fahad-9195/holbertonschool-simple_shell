CC      := gcc
CFLAGS  := -Wall -Werror -Wextra -pedantic -std=gnu89
NAME    := hsh

# خذ كل ملفات C الموجودة تلقائياً (حتى لو ما عندك غير main.c)
SRC     := $(wildcard *.c)

.PHONY: all clean fclean re betty

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

# هدف betty آمن حتى لو ما فيه *.c/*.h
betty:
	@files="$(wildcard *.c) $(wildcard *.h)"; \
	if [ -z "$$files" ]; then \
		echo "No C/H files to check."; \
	else \
		betty $$files; \
	fi
