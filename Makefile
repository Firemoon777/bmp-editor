CC=gcc
CFLAGS= -ansi -c -std=c89 -g -Wall -Werror -I./include/ 
SFLAGS= -shared -std=c89 -g -Wall -Werror -I./include/ -fPIC -lm 
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=build/%.o)
PROJECT_NAME=bmp-editor

all: plugins $(OBJ)
	$(CC) -lm -ldl -lrt -o $(PROJECT_NAME) $(OBJ)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

plugins: bmp_reader.so bmp_writer.so blur.so rotate.so mrotate.so
	
blur.so: 
	$(CC) $(SFLAGS) -o plugins/$@ modules/gaussianblur/*.c
	
bmp_reader.so: 
	$(CC) $(SFLAGS) -o plugins/$@ modules/bmp_reader/*.c

bmp_writer.so: 
	$(CC) $(SFLAGS) -o plugins/$@ modules/bmp_writer/*.c
	
rotate.so: 
	$(CC) $(SFLAGS) -o plugins/$@ modules/rotation/*.c

mrotate.so:
	$(CC) $(SFLAGS) -lpthread -o plugins/$@ modules/pthread_rotation/*.c

run: all
	./build/$(PROJECT_NAME)

clean:
	rm -rf ./build/* 
	rm -rf ./plugins/*
