#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "huffman.h"

#define TOP 0
#define get_parent(id) (id-1)/2
#define get_left_child(id) (id*2)+1
#define get_right_child(id) (id*2)+2


HEAP_TREE* create_heap_tree(){

	HEAP_TREE* tree = (HEAP_TREE*)malloc(sizeof(HEAP_TREE)*1);

	tree->root = NULL;
	tree->height = 0;

	return tree;
}

NO  *create_node(char letter){
	NO* node = (NO*)malloc(sizeof(NO)*1);

	node->heap_left = NULL;
	node->heap_right = NULL;
	node->heap_father = NULL;
	node->huff_left = NULL;
	node->huff_right = NULL;
	node->prob = 1;
	node->letters = (char*)malloc(sizeof(char)*2);
	node->letters[0] = letter;
	node->letters[1] = '\0';
	node->altura = 0;

	return node;
}

void print_fathers(NO* beg, int N){
	printf("letter %c then father\n", beg->letters[0]);
	if(N > 0 && beg->heap_father != NULL){
		print_fathers(beg->heap_father, N-1);
	}
}

void print_heap(NO* node, int pt){
	int i;
	if(node != NULL){
		for(i=0;i<pt;i++) printf("...");
		printf("name:%s prob:%d",node->letters,node->prob);
		printf("\n");
		print_heap(node->heap_left,pt+1);
		print_heap(node->heap_right,pt+1);
	}
}

int contarNos(NO *pRaiz){
   if(pRaiz == NULL)
        return 0;
   else
        return 1 + contarNos(pRaiz->heap_left) + contarNos(pRaiz->heap_right);
}

// search in which position the new element must be added and return its path
int *calc_next_position(HEAP_TREE* tree, int dif){

	int *pos = (int*)malloc(sizeof(int)*(tree->height));
	int i;
	int aux = 0;
	int x = 1;

	for(i=tree->height-1;i>=0;i--){
		aux = dif & x;
		//printf("aux %d dif %d x %d\n", aux, dif, x);
		if(aux > 0)
			pos[i] = 1;
		else
			pos[i] = 0;
		x = x << 1;
	}

	return pos;
}

void insert_heap(HEAP_TREE* tree, NO* new){

	int i, j = 0;
	NO* aux = tree->root;
	int height = tree->height;

	int* pos = NULL;


	int full = pow(2,tree->height+1)-1;
	int have = contarNos(tree->root);
	int dif = full - have;

	printf("tree height = %d\n", tree->height);
	printf("full = %d have = %d dif = %d\n", full, have, dif);


	if(aux == NULL){
		tree->root = new;
	}
	else if(dif == 0){
		while(aux->heap_left != NULL){
			aux = aux->heap_left;
		}
		aux->heap_left = new;
		new->heap_father = aux;
		printf("Novo pai do %c eh %c\n", new->letters[0],new->heap_father->letters[0]);
		tree->height+=1;
	}
	else{	
		dif = have - (pow(2,tree->height)-1);
		// get the path to the place
		pos = calc_next_position(tree,dif);
		printf("pos = ");
		for(i=0;i<tree->height;i++) printf("%d",pos[i]);
		printf("\n");

		// travels the tree according to the path
		for(i=0;i<tree->height-1;i++){
			
			if(pos[i] == 1){
				aux = aux->heap_right;
			}
			else if(pos[i] == 0){
				aux = aux->heap_left;
			}
		}
		// last number says in which way the element is added
		if(pos[tree->height-1] == 0)
			aux->heap_left = new;
		if(pos[tree->height-1] == 1)
			aux->heap_right = new;
		new->heap_father = aux;
		printf("Novo pai do %c eh %c\n", new->letters[0],new->heap_father->letters[0]);
	}


	NO** pais = (NO**)malloc(sizeof(NO*)*(tree->height+1));
	aux = tree->root;
	if(dif != 0 && tree->height > 0){
		for(i=0;i<tree->height;i++){
			pais[i] = aux;
			if(pos[i] == 1){
				aux = aux->heap_right;
			}
			else if(pos[i] == 0){
				aux = aux->heap_left;
			}
		}
		pais[tree->height] = new;
		for(i=0;i<=tree->height;i++){
			printf("pai letra = %c\n", pais[i]->letters[0]);
		}
	}
	else if(dif == 0){
		for(i=0;i<tree->height;i++){
			pais[i] = aux;
			aux = aux->heap_left;
		}
		pais[tree->height] = new;
		for(i=0;i<=tree->height;i++){
			printf("pai letra = %c\n", pais[i]->letters[0]);
		}
	}

	if(tree->height > 0){
		printf("vai corrigir\n");
		correctUp(tree,pais,tree->height+1,tree->height);
	}
	// correct up new element
}

void swap(NO **vector, int filho, int pai) {
	
	NO* aux = NULL; 

	// troca os ponteiros pai
	vector[filho]->heap_father = vector[pai-1];
	vector[pai]->heap_father = vector[filho];
	printf("Novo pai do %c eh %c\n", vector[filho]->letters[0],vector[filho]->heap_father->letters[0]);
	printf("Novo pai do %c eh %c\n", vector[pai]->letters[0],vector[pai]->heap_father->letters[0]);
	
	// Caso o filho seja filho esquerdo pai
	if(vector[pai]->heap_left == vector[filho]){
		aux = vector[pai]->heap_right;
		vector[pai]->heap_right = vector[filho]->heap_right;
		vector[filho]->heap_right = aux;
		
		vector[pai]->heap_left = vector[filho]->heap_left;
		vector[filho]->heap_left = vector[pai];
	}
	else if(vector[pai]->heap_right == vector[filho]){
		aux = vector[pai]->heap_left;
		vector[pai]->heap_left = vector[filho]->heap_left;
		vector[filho]->heap_left = aux;

		vector[pai]->heap_right = vector[filho]->heap_right;
		vector[filho]->heap_right = vector[pai];
	}
	// troca o ponteiro de filho do pai do pai pra ser o pai do filho
	if(vector[pai-1]->heap_left == vector[pai]){
		vector[pai-1]->heap_left = vector[filho];
	}
	else if(vector[pai-1]->heap_right == vector[pai]){
		vector[pai-1]->heap_right = vector[filho];
	}


	// troca a posicao no vetor
	aux = vector[filho];
	vector[filho] = vector[pai];
	vector[pai] = aux;

}

void swaproot(HEAP_TREE* tree,NO** vector, int filho, int pai){
	NO* aux = NULL; 
	// troca os ponteiros pai
	vector[filho]->heap_father = NULL;
	vector[pai]->heap_father = vector[filho];

	if(vector[filho]->heap_father == NULL)
		printf("Novo pai do %c eh NULL\n", vector[filho]->letters[0]);
	printf("Novo pai do %c eh %c\n", vector[pai]->letters[0],vector[pai]->heap_father->letters[0]);
	

	// Caso o filho seja filho esquerdo pai
	if(vector[pai]->heap_left == vector[filho]){
		aux = vector[pai]->heap_right;
		vector[pai]->heap_right = vector[filho]->heap_right;
		vector[filho]->heap_right = aux;
		
		vector[pai]->heap_left = vector[filho]->heap_left;
		vector[filho]->heap_left = vector[pai];
	}// Caso o filho seja o filho direito do pai
	else if(vector[pai]->heap_right == vector[filho]){
		aux = vector[pai]->heap_left;
		vector[pai]->heap_left = vector[filho]->heap_left;
		vector[filho]->heap_left = aux;

		vector[pai]->heap_right = vector[filho]->heap_right;
		vector[filho]->heap_right = vector[pai];
	}

	
	tree->root = vector[filho];

	// troca a posicao no vetor
	aux = vector[filho];
	vector[filho] = vector[pai];
	vector[pai] = aux;

}

void correctUp(HEAP_TREE* tree,NO** pais, int n, int index){
	if(index > 1 && pais[index-1]->prob > pais[index]->prob){
		swap(pais,index,index-1);
		correctUp(tree,pais,n,index-1);
	}
	if(index = 1 && pais[index-1]->prob > pais[index]->prob){
		swaproot(tree,pais,index,index-1);
	}
}

void swaplastroot(HEAP_TREE* tree,NO** pais, int index){

	NO* aux = NULL;

	// troca os ponteiros pai
	pais[index]->heap_father = NULL;
	pais[0]->heap_father = pais[index-1];
	pais[1]->heap_father = pais[index];

	aux = pais[index]->heap_left;
	pais[index]->heap_left = pais[0]->heap_left;
	pais[0]->heap_left = NULL;

	aux = pais[index]->heap_right;
	pais[index]->heap_right = pais[0]->heap_right;
	pais[0]->heap_right = NULL;

	if(pais[index-1]->heap_left = pais[index]){
		pais[index-1]->heap_left = pais[0];
	}
	else if(pais[index-1]->heap_right = pais[index]){
		pais[index-1]->heap_right = pais[0];
	}


	tree->root = pais[index];

	aux = pais[0];
	pais[0] = pais[index];
	pais[index] = aux;

}	

void remove_last(NO** pais, int index){
	if(pais[index-1]->heap_left = pais[index]){
		pais[index-1]->heap_left = NULL;
	}
	else if(pais[index-1]->heap_right = pais[index]){
		pais[index-1]->heap_right = NULL;
	}
}

NO* remove_root(HEAP_TREE* tree){

	int i;
	int *pos = NULL;

	NO* aux = tree->root;

	int full = pow(2,tree->height+1)-1;
	int have = contarNos(tree->root);
	int dif = full - have;

	printf("tree height = %d\n", tree->height);
	printf("full = %d have = %d dif = %d\n", full, have, dif);

	if(dif == 0){

		while(aux->heap_right != NULL){
			aux = aux->heap_right;
		}
		tree->height--;
	}
	else{
		have--;
		dif = have - (pow(2,tree->height)-1);
		// get the path to the place
		pos = calc_next_position(tree,dif);
		printf("pos = ");
		for(i=0;i<tree->height;i++) printf("%d",pos[i]);
		printf("\n");
	}
	NO** pais = (NO**)malloc(sizeof(NO*)*(tree->height+1));
	aux = tree->root;
	if(dif != 0 && tree->height > 0){
		for(i=0;i<=tree->height;i++){
			pais[i] = aux;
			if(pos[i] == 1){
				aux = aux->heap_right;
			}
			else if(pos[i] == 0){
				aux = aux->heap_left;
			}
		}
		//pais[tree->height] = ;
		for(i=0;i<=tree->height;i++){
			printf("pai letra = %c\n", pais[i]->letters[0]);
		}
	}
	else if(dif == 0){
		for(i=0;i<=tree->height;i++){
			pais[i] = aux;
			aux = aux->heap_left;
		}
		//pais[tree->height] = new;
		for(i=0;i<=tree->height;i++){
			printf("pai letra = %c\n", pais[i]->letters[0]);
		}
	}

	swaplastroot(tree,pais,tree->height);
	remove_last(pais,tree->height);
	printf("vai corrigir pra baixo\n");
	correctDown(tree,tree->root);
	return pais[tree->height];
}


void swaplargestson(HEAP_TREE* tree,NO* father, NO* son){
	NO* aux;

	if(tree->root == father){
		tree->root = son;
	}
	if(son == father->heap_left){
		aux = father->heap_right;
		father->heap_right = son->heap_right;
		son->heap_right = aux;

		father->heap_left = son->heap_left;
		son->heap_left = father;
		
		if(father->heap_father!=NULL){
			if(father->heap_father->heap_left == father)
				father->heap_father->heap_left = son;
			else if(father->heap_father->heap_right == father)
				father->heap_father->heap_right = son;
		}
		son->heap_father = father->heap_father;
		father->heap_father = son;
	}
	else if(son == father->heap_right){
		aux = father->heap_left;
		father->heap_left = son->heap_left;
		son->heap_left = aux;

		father->heap_right = son->heap_right;
		son->heap_right = father;
		if(father->heap_father != NULL){
			if(father->heap_father->heap_left == father)
				father->heap_father->heap_left = son;
			else if(father->heap_father->heap_right == father)
				father->heap_father->heap_right = son;
		}
		son->heap_father = father->heap_father;
		father->heap_father = son;
	}
}

void correctDown(HEAP_TREE* tree,NO* root){

	if(root->heap_left == NULL && root->heap_right == NULL) return;
	int left_prob = 0;
	int right_prob = 0;
	printf("corrigindo letra %c\n", root->letters[0]);
	//print_heap(tree->root,0);

	if(root->heap_left != NULL){
		left_prob = root->heap_left->prob;	 
		printf("achou esquedo com valor %d\n", left_prob);
	}
	if(root->heap_right != NULL){
		right_prob = root->heap_right->prob;
		printf("achou direito com valor %d\n", right_prob);
	}
	NO* lesser_prob = root;

	if (left_prob < lesser_prob->prob){
		printf("esquerdo maior\n");
		lesser_prob = root->heap_left;
	}
	if (right_prob < lesser_prob->prob){
		printf("direito maior\n");
		lesser_prob = root->heap_right;
	}

	if(lesser_prob != root){
		printf("vai trocar %c com filho %c\n", root->letters[0],lesser_prob->letters[0]);
		swaplargestson(tree,root,lesser_prob);
		printf("Vai continuar corrigindo\n");
		correctDown(tree,root);
	}

}






/*
void correctUp(NO** vector, int n, int index){
	{
		// Enquanto o valor do filho for menor do que o valor do pai, troca o pai com o filho e sobe
		for (; index > 1 && vector[index] < vector[get_parent(index)]; index = get_parent(index))
			swap(vector,index,get_parent(index));
	}
}
*/
void correctUp2(NO** vector,int index){
	// Se index não é a raiz e o valor do index for menor do que o valor de seu pai,
	// troca seus valores (index e pai(index)) e corrige para o pai
	if (index > 0 && vector[index]->prob > vector[index-1]->prob){
		swapheapsort(vector,index,index-1);
		correctUp2(vector,index-1);
	}
}

void max_heapify(NO **vector, int n, int index) {

	int left_id = get_left_child(index);	 
	int right_id = get_right_child(index);   
	int largest_id = index;			 

	if (left_id < n && vector[left_id]->prob < vector[largest_id]->prob) 
		largest_id = left_id;
	if (right_id < n && vector[right_id]->prob < vector[largest_id]->prob) 
		largest_id = right_id;

	if (largest_id != index) {
		swap(vector, index, largest_id);
		max_heapify(vector, n, largest_id);
	}
}

void build_max_heap(NO **vector, int n) {
	int index;

	for (index = get_parent(n-1); index >= 0; index--) {
		max_heapify(vector, n, index);
	}
}

void swapheapsort(NO **vector, int i, int j){
	NO* aux = vector[i];
	vector[i] = vector[j];
	vector[j] = aux;
}

void heapsort_(NO **vector, int n) {
	int i;
	
	build_max_heap(vector, n);

	for (i = 1; i < n; i++) {
		swapheapsort(vector, TOP, n-i);
		max_heapify(vector, n-i, TOP);
	}
}