#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "heapsort.h"

typedef struct tabelahuffman TABELA_HUFFMAN;
typedef struct element ELEMENTS;

struct element{
	char letter;
	char *bin;
	int size_bin;
};

struct tabelahuffman{
	int n_elements;
	ELEMENTS** vet;
};


NO* buildHuffmanTree(char* text);
TABELA_HUFFMAN* criarTabelaHuffman();

void fill_table(TABELA_HUFFMAN* tabela,NO* raiz, int n_letters);
int compactarHuffman(char* text, char** textCompac, TABELA_HUFFMAN** tabela);
char *descompactarHuffman(char *textCompac, int sizeCompac, TABELA_HUFFMAN* tabela);

#endif // _HUFFMAN_H_
