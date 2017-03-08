VPATH=src
CC=gcc
CFLAGS=-O3 -Wall
LINK=-lm -lgmp -lmpfr
INC=-I/opt/local/include 
LIB=-L/opt/local/lib

npp-tree: npp-tree.c
	$(CC) $(CFLAGS) $(INC) $(LIB) src/npp-tree.c -o bin/npp-tree.exe $(LINK)

karmarkar-karp: karmarkar-karp.c
	$(CC) $(CFLAGS) $(INC) $(LIB) src/karmarkar-karp.c -o bin/karmarkar-karp.exe $(LINK)

kk-mpfr: karmarkar-karp-mpfr.c
	$(CC) $(CFLAGS) $(INC) $(LIB) src/karmarkar-karp-mpfr.c -o bin/kk-mpfr.exe $(LINK)


# Other functions
clean :
	rm -rf *~ *.o *.exe
