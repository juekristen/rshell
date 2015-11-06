CC=g++
CFlags=-Wall -Werror -ansi -pedantic

all:
	rm -rf bin
	mkdir bin
	$(CC) $(CFlags) src/rshell.cpp -o bin/rshell

rshell: rshell.cpp
	$(CC) $(CFlags) src/rshell.cpp -o bin/rshell