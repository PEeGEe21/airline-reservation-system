CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
SRC_EXTRA = include/input_utils.c

all: main

%: %.c $(SRC_EXTRA)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "compiled successfully"

clean:
	rm -f main *.o

run: main
	./main