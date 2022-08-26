CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lm

OBJFILES = rng.o rngs.o rvgs.o rvms.o 

all: $(OBJFILES)

progetto:
	gcc -g -O -c Simulatore_Migliorato.c
	gcc -g -O -c librerieProgetto/utils.c
	gcc -o main Main.c -Werror -Wextra -Wall -lm Simulatore.o Simulatore_Migliorato.o utils.o libreriaRand/rngs.o libreriaRand/rvms.o
	./main

clean:
	/bin/rm -f $(OBJFILES) core*
