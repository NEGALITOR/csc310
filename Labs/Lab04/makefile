# This is a comment line
CC=g++
# CFLAGS will be the options passed to the compiler.
CFLAGS= -c

all: mp3

prog: mp3.o 
	$(CC) mp3.o -o mp3

mp3.o: mp3.cpp
	$(CC) $(CFLAGS) mp3.cpp

clean:
	rm -rf *.o
