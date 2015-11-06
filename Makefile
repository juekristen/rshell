CC=g++
CFlags=-Wall -Werror -ansi -pedantic

all: files

files: rshell.cpp
	$(CC) $(CFlags) rshell.cpp

rshell: rshell.cpp
	$(CC) $(CFlags) rshell.cpp