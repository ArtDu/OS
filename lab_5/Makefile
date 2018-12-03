CC = gcc
FLAGS = -std=c99 -w -Werror -Wall -pedantic

all: run

run: libtree.so mainStat.o mainDyn.o
	$(CC) $(FLAGS) -o stat mainStat.o -L. -ltree -Wl,-rpath,.
	$(CC) $(FLAGS) -o dyn mainDyn.o -ldl -Wl,-rpath,.

mainStat.o: mainStat.c
	$(CC) -c $(FLAGS) mainStat.c

mainDyn.o: mainDyn.c
	$(CC) -c $(FLAGS) mainDyn.c

libtree.so: Tree.o
	$(CC) $(FLAGS) -shared -o libtree.so Tree.o

Tree.o: Tree.c
	$(CC) -c -fPIC $(FLAGS) Tree.c

clean:
	rm -f *.o stat dyn *.so *.a
