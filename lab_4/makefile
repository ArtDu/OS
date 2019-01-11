CC = gcc
FLAGS = -std=gnu11 -fpermissive -w -Werror -Wall -pedantic

all: run

run: main.o vector.o
	$(CC) $(FLAGS) -o main main.o vector.o

vector.o: vector.cpp
	$(CC) -c -$(FLAGS) vector.cpp

main.o: main.c
	$(CC) -c -$(FLAGS) main.c

clean:
	rm -f *.o
