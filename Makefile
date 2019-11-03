all:dph prod cons mycall
	

	
dph:
	gcc -o dph dph.c -lrt -lpthread

	
prod:
	gcc -o prod prod.c -lrt -lpthread -lm

	
cons:
	gcc -o cons cons.c -lrt -lpthread -lm

	
mycall:
	gcc -o mycall mycall.c
