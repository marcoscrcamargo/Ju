#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "heapsort.h"

#define get_left_child(id) (id*2)+1
#define get_right_child(id) (id*2)+2

TABELA_HUFFMAN *criarTabelaHuffman(){

	TABELA_HUFFMAN *tabela =(TABELA_HUFFMAN*)malloc(sizeof(TABELA_HUFFMAN)*1);
	//tabela->vet = (ELEMENTS*)calloc(1,sizeof(ELEMENTS));
	tabela->vet = (ELEMENTS*)calloc(4,sizeof(ELEMENTS));
	tabela->n_elements = 0;

	tabela->vet[0].letter = 'A';
	tabela->vet[0].size_bin = 2;
	tabela->vet[0].bin = (char *)malloc(sizeof(char)*2);
	tabela->vet[0].bin[0] = '0';	
	tabela->vet[0].bin[1] = '1';	
	tabela->vet[1].letter = 'B';
	tabela->vet[1].size_bin = 2;
	tabela->vet[1].bin = (char *)malloc(sizeof(char)*2);
	tabela->vet[1].bin[0] = '1';	
	tabela->vet[1].bin[1] = '0';	
	tabela->vet[2].letter = 'N';
	tabela->vet[2].size_bin = 2;
	tabela->vet[2].bin = (char *)malloc(sizeof(char)*2);
	tabela->vet[2].bin[0] = '1';
	tabela->vet[2].bin[1] = '1';
	tabela->vet[3].letter = ' ';
	tabela->vet[3].size_bin = 3;
	tabela->vet[3].bin = (char *)malloc(sizeof(char)*3);
	tabela->vet[3].bin[0] = '0';	
	tabela->vet[3].bin[1] = '0';	
	tabela->vet[3].bin[2] = '1';	
	tabela->n_elements=4;

	return tabela;
}

void print_tree(NO* root, int pt){
	int i;
	if(root != NULL){
		for(i=0;i<pt;i++) printf("...");
		printf("name:%s prob:%d",root->letters,root->prob);
		printf("\n");
		print_tree(root->left,pt+1);
		print_tree(root->right,pt+1);
	}
}
void print_heap(NO** vector, int index, int n, int pt){
	int i;
	if(index < n){
		for(i=0;i<pt;i++) printf("...");
		printf("name:%s prob:%d",vector[index]->letters,vector[index]->prob);
		printf("\n");
		print_heap(vector,get_left_child(index),n,pt+1);
		print_heap(vector,get_right_child(index),n,pt+1);
	}
}

NO  *create_node(char letter){
	NO* node = (NO*)malloc(sizeof(NO)*1);

	node->left = NULL;
	node->right = NULL;
	node->prob = 1;
	node->letters = (char*)malloc(sizeof(char)*2);
	node->letters[0] = letter;
	node->letters[1] = '\0';

	return node;
}

NO ** add_letter(char letter, NO** node_vector, int* n_letters){

	int i;

	// Procura a letra a ser adicionada no vetor de Nos
	for(i=0;i<(*n_letters);i++){
		// se existir a letra, aumenta a probabilidade
		if(node_vector[i]->letters[0] == letter){
			node_vector[i]->prob += 1;
			return node_vector;
		}
	}

	// se chegar aqui, nao existe a letra no vetor de Nos

	node_vector = (NO**)realloc(node_vector,sizeof(NO *)*((*n_letters)+1));
	node_vector[(*n_letters)] = create_node(letter);
	(*n_letters)+=1;

	return node_vector;
}

NO* make_father(NO* left, NO* right){
	NO* father = (NO*)malloc(sizeof(NO)*1);

	father->left = left;
	father->right = right;
	father->prob = left->prob + right->prob;
	father->letters = (char*)malloc(sizeof(char)*((int)strlen(left->letters)+(int)strlen(right->letters)+1));
	strcpy(father->letters,left->letters);
	strcat(father->letters,right->letters);
	
	return father;
}

/*
NO* buildHuffmanTree(char* text){

	int text_size = (int)strlen(text);
	int i, n_letters=0;

	// Cria vetor com um No para cada letra diferente do texto
	NO** node_vector = NULL;
	printf("Vai adicionar letras\n");
	for(i=0;i<text_size;i++){
		printf("Adicionando letra %c\n",text[i]);
		node_vector = add_letter(text[i],node_vector, &n_letters);
	}
	printf("Nao ordenado\n");
	for(i=0;i<n_letters;i++){
		printf("letra %c com prob. %d\n", node_vector[i].letters[0],node_vector[i].prob);
	}

	NO **heap_vector = NULL;
	int n_heap = 0;
	for(i=0;i<n_letters;i++){
		heap_vector = (NO**)realloc(heap_vector,sizeof(NO*)*(n_heap+1));
		heap_vector[n_heap] = node_vector[i];
		correctUp(heap_vector,n_heap);
		n_heap++;
	}

	print_heap(heap_vector,0,n_heap,0);

	while(n_heap>0){
		
	}

	
	// Ordena o Vetor de Nos
	heapsort_(node_vector,n_letters);

	printf("\nOrdenado\n");
	for(i=0;i<n_letters;i++){
		printf("letra %c com prob. %d\n", node_vector[i].letters[0],node_vector[i].prob);
	}

	NO* father = NULL;
	while(n_letters>1){
		n_letters--;
		father = make_father(node_vector[n_letters-1],node_vector[n_letters]);
		node_vector = realloc(node_vector,sizeof(NO*)*(n_letters));
		node_vector[n_letters-1] = father;
		correctUp(node_vector,n_letters-1);
		printf("vetor depois de por o pai com prob = %d\n",father->prob);
		for(i=0;i<n_letters;i++){
			printf("letra %s com prob. %d\n", node_vector[i].letters,node_vector[i].prob);
		}
		//n_letters--;
	}	

	//printf("raiz letras = %s e prob = %d\n", node_vector[0].letters,node_vector[0].prob);
	print_tree(node_vector[0],0);
	
{}*/

int compactarHuffman(char* text, char **textCompac, TABELA_HUFFMAN **tabela){

	(*textCompac) = NULL;
	int size=0, sizeCompac=0, count=0;
	int i, j, k=0, l, lsize=0;
	int tam = (int)strlen(text);

	for (i=0; i<tam; i++){
		printf("SEGURA RAPAZ\n");
		for(j=0; j<(*tabela)->n_elements; j++){
	    	if(text[i] == (*tabela)->vet[j].letter){
	      		lsize = (*tabela)->vet[j].size_bin;
	      		size += lsize;
	        	if (size%8==0) sizeCompac++;
	        	(*textCompac) = (char *)realloc((*textCompac), size*sizeof(char));
	        	for (k=k; k<size; k++){
	          		for(l=0; l<lsize; l++){
	            		(*textCompac)[k] = (*tabela)->vet[j].bin[l];
	            		//printf("%c\n", (*textCompac)[k]);
	            		//exit(0);
	          		} 
	        	}
	      	}
	    }
	}

	if(size%8!=0){
	    do{
	    	size++;
	        (*textCompac) = (char *)realloc((*textCompac), size*sizeof(char));
	        (*textCompac)[size]='0';
	    }while(size%8!=0);
	    for (i=size-8; i<=size; i++){
	        if((*textCompac)[i]=='1') count++;    
	    }
	    (*textCompac) = (char *)realloc((*textCompac), size+1*sizeof(char));
	    (*textCompac)[size+1]=(char)count;
	    return sizeCompac+=2;
	} 
	else return sizeCompac;
}

char *descompactarHuffman(char *textCompac, int sizeCompac, TABELA_HUFFMAN* tabela){

	int i, j=0, k=0, size=0, count=0;
	char *textDescompac = NULL;
	char *aux;

	do{
		size = strlen(tabela->vet[j].bin);
		aux = (char *)malloc(sizeof(char)*size);
		for (i=0; i<size; i++){
			aux[i] = textCompac[k];
			k++;
		}
		if(strcmp(aux,tabela->vet[j].bin)==0){
			textDescompac = (char *)realloc(textDescompac, (count+1)*sizeof(char));
			textDescompac[count]=tabela->vet[j].letter;
			count++;
		}
		else{
			k=0;
			j++;	
		} 
	}while(textCompac[k]=='1' || textCompac[k]=='0');

	return textDescompac;
}