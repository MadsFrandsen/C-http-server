#GCC=gcc -O3 -g -Wall -Wextra -pedantic -std=gnu11
GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11
LD_FLAGS= -lpthread

all: main

rebuild: clean all

server.o: src/server.c include/server.h
	$(GCC) -c $< -o $@

get_file.o: src/get_file.c include/get_file.h
	$(GCC) -c $< -o $@

routes.o: src/routes.c include/routes.h
	$(GCC) -c $< -o $@

main: src/main.c get_file.o server.o routes.o
	$(GCC) $< *.o -o $@ $(LD_FLAGS)

clean: 
	rm -rf *.o main
