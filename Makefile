all:dph prod cons mycall
	
dph:dph.o
	
dph.o:
	gcc -o dph.o dph.c -lrt -lpthread
prod:prod.o
	
prod.o:
	gcc -o prod.o prod.c -lrt -lpthread -lm
cons:cons.o
	
cons.o:
	gcc -o cons.o cons.c -lrt -lpthread -lm
mycall:mycall.o
	
mycall.o:
	gcc -o mycall.o mycall.c
