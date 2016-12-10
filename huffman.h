#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

typedef struct element ELEMENTS;

struct element{
	char letter;
	char *bin;
	int size_bin;
};

typedef struct tabelahuffman TABELA_HUFFMAN;

struct tabelahuffman{
	int n_elements;
	ELEMENTS* vet;
};

typedef struct node NO;

struct node{
	int prob;
	char* letters;
	NO* left;
	NO* right;
};

NO* buildHuffmanTree(char* text);
NO* create_node(char letter);
TABELA_HUFFMAN* criarTabelaHuffman();
int compactarHuffman(char* text, char **textCompac, TABELA_HUFFMAN **tabela);
char *descompactarHuffman(char *textCompac, int sizeCompac, TABELA_HUFFMAN* tabela);

#endif // _HUFFMAN_H_