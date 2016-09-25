CC=gcc
CFLAGS=-g -c -ansi -pedantic -Wall -Werror
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=bin/%.o)
PROJECT_NAME=bmp-editor

all: $(OBJ)
	$(CC) $(OBJ) -o bin/$(PROJECT_NAME)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./bin/$(PROJECT_NAME)

clean:
	rm -f ./bin/* 
