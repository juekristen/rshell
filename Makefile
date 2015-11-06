CC=g++
CFlags=-c -Wall -Werror -ansi -pedantic

all: files

files: rshell.cpp
	$(CC) $(CFlags) rshell.cpp

rshell: rshell.cpp
	$(CC) $(CFlags) rshell.cpp