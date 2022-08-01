# January 5, 2005
#
# Makefile for compiling accompanying software for
# "Discrete-Event Simulation: A First Course" by Park and Leemis
#

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lm

OBJFILES = rng.o rngs.o rvgs.o rvms.o 

all: $(OBJFILES)

acs: acs.c
	$(CC) $(LDFLAGS) $< -o $@

buffon: buffon.c rng.o
	$(CC) $(LDFLAGS) $^ -o $@

cdh: cdh.c
	$(CC) $(LDFLAGS) $^ -o $@

craps: craps.c rng.o
	$(CC) $(LDFLAGS) $^ -o $@

det: det.c rng.o
	$(CC) $(LDFLAGS) $^ -o $@

ddh: ddh.c
	$(CC) $(LDFLAGS) $^ -o $@

estimate: estimate.c rngs.o rvms.o
	$(CC) $(LDFLAGS) $^ -o $@

galileo: galileo.c rng.o
	$(CC) $(LDFLAGS) $^ -o $@

hat: hat.c rng.o
	$(CC) $(LDFLAGS) $^ -o $@

msq: msq.c rngs.o
	$(CC) $(LDFLAGS) $^ -o $@

rng.o: rng.c rng.h
	$(CC) -c $<

rngs.o: rngs.c rngs.h
	$(CC) -c $<

rvgs.o: rvgs.c rvgs.h rngs.c rngs.h
	$(CC) -c $<

rvms.o: rvms.c rvms.h rngs.c rngs.h
	$(CC) -c $<

san: san.c rngs.o
	$(CC) $(LDFLAGS) $^ -o $@

sieve: sieve.c
	$(CC) $(LDFLAGS) $^ -o $@

sis1: sis1.c
	$(CC) $(LDFLAGS) $^ -o $@

sis2: sis2.c rng.o
	$(CC) $(LDFLAGS) $^ -o $@

sis3: sis3.c rngs.o
	$(CC) $(LDFLAGS) $^ -o $@

sis4: sis4.c rngs.o rvgs.o
	$(CC) $(LDFLAGS) $^ -o $@

ssms: ssms.c rngs.o
	$(CC) $(LDFLAGS) $^ -o $@

ssq1: ssq1.c
	$(CC) $(LDFLAGS) $^ -o $@

ssq2: ssq2.c rng.o
	$(CC) $(LDFLAGS) $^ -o $@

ssq3: ssq3.c rngs.o
	$(CC) $(LDFLAGS) $^ -o $@

ssq4: ssq4.c rngs.o rvgs.o
	$(CC) $(LDFLAGS) $^ -o $@

ttr:  ttr.c rngs.o
	$(CC) $(LDFLAGS) $^ -o $@

uvs: uvs.c rngs.o rvgs.o
	$(CC) $(LDFLAGS) $^ -o $@
matteo: ../Main.c  rngs.o 
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	/bin/rm -f $(OBJFILES) core*
