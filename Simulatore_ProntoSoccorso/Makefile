CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lm

OBJFILES = rng.o rngs.o rvgs.o rvms.o 

all: $(OBJFILES)

progetto: 
	gcc -g -O -c Simulatore.c
	gcc -o main Main.c -lm Simulatore.o libreriaRand/rngs.o libreriaRand/rvms.o
	./main
clean:
	/bin/rm -f $(OBJFILES) core*
