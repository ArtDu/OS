CC = gcc
FLAGS = -std=c99 -w -Werror -Wall -pedantic

all: run

stat: mainStat.o libtree.a
	$(CC) $(FLAGS) -o stat mainStat.o -L. -ltree

dyn: mainDyn.o libtree.so
	$(CC) $(FLAGS) -o dyn mainDyn.o -L. -ltree -Wl,-rpath,. -ldl

mainStat.o: mainStat.c
	$(CC) -c $(FLAGS) mainStat.c

mainDyn.o: mainDyn.c
	$(CC) -c $(FLAGS) mainDyn.c

libtree.a: TreeStat.o
	ar cr libtree.a TreeStat.o

libtree.so: TreeDyn.o
	$(CC) $(FLAGS) -shared -o libtree.so TreeDyn.o

TreeStat.o: Tree.c
	$(CC) -c $(FLAGS) Tree.c -o TreeStat.o

TreeDyn.o: Tree.c
	$(CC) -c -fPIC $(FLAGS) Tree.c -o TreeDyn.o

clean:
	rm -f *.o stat dyn *.so *.a
