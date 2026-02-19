# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -O2 -fPIC
LDFLAGS = 

# Target names
LIB_NAME = libmini_malloc.a
EXE_NAME = test_mini_malloc

# Source files
SRC = mini_malloc.c arena.c
OBJ = $(SRC:.c=.o)
TEST_SRC = test_mini_malloc.c

# Default target
all: $(EXE_NAME)

# Build the executable
$(EXE_NAME): $(OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build the static library
static: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	ar rcs $@ $^

# Pattern rule for .o files
%.o: %.c mini_malloc.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run the test
run: $(EXE_NAME)
	./$(EXE_NAME)

# Benchmark 
benchmark: $(OBJ) benchmark.c
	$(CC) $(CFLAGS) $^ -o benchmark $(LDFLAGS)
	./benchmark

# Clean up
clean:
	rm -f *.o $(EXE_NAME) $(LIB_NAME) benchmark

# Rebuild everything
re: clean all

.PHONY: all static clean run re
