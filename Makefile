VPATH=src
CC=gcc
CFLAGS=-O3 -Wall
LINK=-lm -lgmp -lmpfr
INC=-I/opt/local/include 
LIB=-L/opt/local/lib

create_numbers: create_numbers.c
	$(CC) $(CFLAGS) $(INC) $(LIB) src/create_numbers.c src/npp-io.c -o bin/create_numbers.exe $(LINK)

npp-experiment: npp-experiment.c
	$(CC) $(CFLAGS) $(INC) $(LIB) src/npp-experiment.c src/npp-io.c src/npp-tree.c -o bin/npp-experiment.exe $(LINK)

karmarkar-karp: karmarkar-karp.c
	$(CC) $(CFLAGS) $(INC) $(LIB) src/karmarkar-karp.c -o bin/karmarkar-karp.exe $(LINK)

kk-mpfr: karmarkar-karp-mpfr.c
	$(CC) $(CFLAGS) $(INC) $(LIB) src/karmarkar-karp-mpfr.c -o bin/kk-mpfr.exe $(LINK)


# Other functions
clean :
	rm -rf *~ *.o *.exe
