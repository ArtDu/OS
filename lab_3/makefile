CC = gcc
FLAGS = -std=c99 -lpthread -pthread -fpermissive -w -Werror -Wall -pedantic

all: run

run: main.o
	$(CC) $(FLAGS) -o main main.o

main.o: main.c
	$(CC) -c -$(FLAGS) main.c

clean:
	rm -f *.o
