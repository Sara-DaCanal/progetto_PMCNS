CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lm

OBJFILES = rng.o rngs.o rvgs.o rvms.o

all: $(OBJFILES)

progetto:
	/bin/rm  -d -r statistiche &2>/dev/null
	mkdir target  &2>/dev/null
	gcc -g -O -o target/rvms.o -c librerieProgetto/rvms.c
	gcc -g -O -o target/rngs.o -c librerieProgetto/rngs.c
	gcc -g -O -o target/Simulatore.o -c Simulatore.c
	gcc -g -O -o target/Simulatore_Migliorato.o -c Simulatore_Migliorato.c
	gcc -g -O -o target/utils.o -c librerieProgetto/utils.c
	clear
	#gcc -o P percentuale.c -Werror -Wextra -Wall -lm target/Simulatore.o target/Simulatore_Migliorato.o target/utils.o target/rngs.o target/rvms.o
	#./P
	gcc -o main Main.c -Werror -Wextra -Wall -lm target/Simulatore.o target/Simulatore_Migliorato.o target/utils.o target/rngs.o target/rvms.o
	./main
	rm main	
	/bin/rm  -d -r target &2>/dev/null