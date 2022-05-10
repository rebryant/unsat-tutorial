CFLAGS=-std=c99 -O2 -g

all: drat-trim lrat-check 

drat-trim: drat-trim.c
	gcc drat-trim.c $(CFLAGS) -o drat-trim

lrat-check: lrat-check.c
	gcc lrat-check.c -std=c99 $(CFLAGS) -o lrat-check


clean:
	rm -f drat-trim lrat-check
	rm -f *~
	rm -rf *.dSYM
