#Makefile :: grafo

all: heapsort.o huffman.o trab4.o
	 gcc -o T4 heapsort.o huffman.o trab4.o -lm -g -Wall

heapsort.o: heapsort.c
	gcc -c heapsort.c -g

huffman.o: huffman.c
	gcc -c huffman.c -g -O2

trab4.o: trab4.c
	gcc -c trab4.c -g
	
run:
	./T4
	
valgrind:
	valgrind --leak-check=full --track-origins=yes ./T4
	
clean:
	rm *.o

zip:
	zip Projeto huffman.c huffman.h heapsort.c heapsort.h trab4.c Makefile

codes: all clean zip
