CC = gcc
CFLAGS = -Wall -Wextra -g -O2

all: test

test: arena.o mini_malloc.o test.o
	$(CC) $(CFLAGS) $^ -o test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o test
