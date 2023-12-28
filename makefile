#GCC=gcc -O3 -g -Wall -Wextra -pedantic -std=gnu11
GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11
LD_FLAGS= -lpthread

all: main

rebuild: clean all

server.o: server.c server.h
	$(GCC) -c $< -o $@

get_file.o:
	$(GCC) -c $< -o $@

main: main.c get_file.o server.o
	$(GCC) $< *.o -o $@ $(LD_FLAGS)

clean: 
	rm -rf *.o main