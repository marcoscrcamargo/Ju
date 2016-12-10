#ifndef _HEAPSORT_H_
#define _HEAPSORT_H_


typedef struct node NO;

typedef struct heap_tree{
	NO* root;
	int height;
}HEAP_TREE;

struct node{
	int prob;
	char* letters;
	NO* heap_left;
	NO* heap_right;
	NO* heap_father;
	NO* huff_left;
	NO* huff_right;
	int altura;
};


HEAP_TREE* create_heap_tree();
NO  *create_node(char letter);

void print_heap(NO* node, int pt);

int count_nodes(HEAP_TREE* tree);
void insert_heap(HEAP_TREE* tree, NO* new);
NO* remove_root(HEAP_TREE* tree);

void correctUp(HEAP_TREE* tree,NO** pais, int n, int index);
void correctUp2(NO** vector,int index);
void correctDown(HEAP_TREE* tree,NO* root);

void swapheapsort(NO **vector, int i, int j);

//void correctUp(NO** vector, int index);
void heapsort_(NO **, int);

#endif
