CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)

all: bin/toolkit

bin/toolkit: $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o bin/toolkit

clean:
	rm -rf bin
