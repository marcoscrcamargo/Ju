# Makefile :: 

# Search for .c source files
SRC=$(wildcard *.c)
# Search for .h header files
DEPS=$(wildcard *.h)
# Generate .o object files rules
OBJ=$(foreach file, $(SRC), $(file:%.c=%.o))

all: main
	gcc -o T4 huffman.o trab4.o heapsort.o -lm -g -Wall

%.o: %.c $(DEPS)
	gcc -c -o $@ $< -lm -g -Wall -O2

main: $(OBJ)
	gcc -o T4 huffman.o trab4.o heapsort.o -lm -g -Wall -O2

run:
	./T4
	
valgrind:
	valgrind --leak-check=full --track-origins=yes ./T4
	
clean:
	-rm -f T4 *.zip

cleanobj:
	-rm -f *.o

zip: clean cleanobj
	zip Projeto *

codes: all zip
