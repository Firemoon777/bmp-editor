CC=gcc
CFLAGS= -c -std=c89 -g -pedantic -Wall -Werror -I./include/
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=build/%.o)
PROJECT_NAME=bmp-editor

all: $(OBJ)
	$(CC) -lm -o build/$(PROJECT_NAME) $(OBJ)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


run: all
	./build/$(PROJECT_NAME)

clean:
	rm -rf ./build/* 
