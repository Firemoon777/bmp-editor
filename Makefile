CC=gcc
ASM=nasm
ASMFLAGS=-f elf64 -g
CFLAGS= -O3 -ansi -c -std=c11 -g -march=x86_64 -Wall -Werror -I./include/ 
SFLAGS= -shared -O3 -march=x86_64 -std=c11 -g -Wall -Werror -I./include/ -fPIC -lm 
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=build/%.o)
PROJECT_NAME=bmp-editor

all: plugins $(OBJ)
	$(CC) -lm -ldl -lrt -o $(PROJECT_NAME) $(OBJ)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

plugins: bmp_reader.so bmp_writer.so blur.so rotate.so mrotate.so sepia_c.so sepia_asm.so
	
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
	
sepia_c.so:
	$(CC) $(SFLAGS) -o plugins/$@ modules/sepia_c/*.c
	
sepia_asm.so:
	$(ASM) $(ASMFLAGS) modules/sepia_asm/sepia.asm -o modules/sepia_asm/sepia.o
	$(CC) $(SFLAGS) -o plugins/$@ modules/sepia_asm/*.c modules/sepia_asm/sepia.o

run: all
	./build/$(PROJECT_NAME)

clean:
	rm -rf ./build/* 
	rm -rf ./plugins/*
