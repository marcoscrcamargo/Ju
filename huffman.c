#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "huffman.h"
#include "heapsort.h"

#define get_left_child(id) (id*2)+1
#define get_right_child(id) (id*2)+2

#define TRUE 1
#define FALSE 0

TABELA_HUFFMAN* criarTabelaHuffman(){

	TABELA_HUFFMAN* tabela = (TABELA_HUFFMAN*)malloc(sizeof(TABELA_HUFFMAN)*1);
	tabela->vet = NULL;
	tabela->n_elements = 0;

	return tabela;
}

ELEMENTS* create_element(){

	ELEMENTS* element = (ELEMENTS*)malloc(sizeof(ELEMENTS)*1);

	element->letter = -1;
	element->bin = NULL;
	element->size_bin = 0;

	return element;
}

/*void print_table(TABELA_HUFFMAN* tabela){

	int i;

	for(i=0;i<tabela->n_elements;i++){
		printf("letra %c com bin %s\n", tabela->vet[i]->letter,tabela->vet[i]->bin);
	}
}*/

/*void print_tree(NO* root, int pt){
	int i;
	if(root != NULL){
		for(i=0;i<pt;i++) printf("...");
		printf("name:%s prob:%d",root->letters,root->prob);
		printf("\n");
		print_tree(root->heap_left,pt+1);
		print_tree(root->heap_right,pt+1);
	}
}*/

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

	father->heap_left = left;
	father->heap_right = right;
	father->prob = left->prob + right->prob;
	father->letters = (char*)malloc(sizeof(char)*((int)strlen(left->letters)+(int)strlen(right->letters)+1));
	strcpy(father->letters,left->letters);
	strcat(father->letters,right->letters);
	
	return father;
}

int compactarHuffman(char* text, char** textCompac, TABELA_HUFFMAN** tabela){

	int text_size = (int)strlen(text);
	int i, n_letters=0;
	int keep;

	// Cria vetor com um No para cada letra diferente do texto
	NO** node_vector = NULL;
	//printf("Vai adicionar letras\n");
	for(i=0;i<text_size;i++){
		//printf("Adicionando letra %c\n",text[i]);
		node_vector = add_letter(text[i],node_vector, &n_letters);
	}
	keep = n_letters;
	/*
	HEAP_TREE* tree = create_heap_tree();
	for(i=0;i<n_letters;i++){
		printf("inserindo letra %c\n", node_vector[i]->letters[0]);
		insert_heap(tree, node_vector[i]);
		print_heap(tree->root,0);
		printf("\n\n");
	}

	//while(tree->root->heap_left != NULL || tree->root->heap_right != NULL){
	for(i=0;i<5;i++){
		printf("vai remover\n");
		NO* rem1 = remove_root(tree);
		printf("vai remover segunda vez\n");
		NO* rem2 = remove_root(tree);
		print_heap(tree->root,0);
		printf("removeu o no %c\n", rem1->letters[0]);
		//printf("removeu o no %c\n", rem2->letters[0]);
	}
	*/
	//print_heap(tree->root,0);
	//print_heap(heap_vector,0,n_heap,0);

	
	// Ordena o Vetor de Nos
	heapsort_(node_vector,n_letters);

	NO* father = NULL;
	while(n_letters>1){
		n_letters--;
		father = make_father(node_vector[n_letters-1],node_vector[n_letters]);
		node_vector = realloc(node_vector,sizeof(NO*)*(n_letters));
		node_vector[n_letters-1] = father;
		correctUp2(node_vector,n_letters-1);
		//n_letters--;
	}	

	//printf("raiz letras = %s e prob = %d\n", node_vector[0]->letters,node_vector[0]->prob);
	fill_table(*tabela,node_vector[0],keep);

	(*textCompac) = NULL;
	int size=0, sizeCompac=0, count=0;
	int j, k=0, l, lsize=0;
	int tam = (int)strlen(text);

	for (i=0; i<tam; i++){
		printf("SEGURA RAPAZ\n");
		for(j=0; j<(*tabela)->n_elements; j++){
	    	if(text[i] == (*tabela)->vet[j]->letter){
	      		lsize = (*tabela)->vet[j]->size_bin;
	      		size += lsize;
	        	if (size%8==0) sizeCompac++;
	        	(*textCompac) = (char *)realloc((*textCompac), size*sizeof(char));
	        	for (k=k; k<size; k++){
	          		for(l=0; l<lsize; l++){
	            		(*textCompac)[k] = (*tabela)->vet[j]->bin[l];
	            		//printf("COMPACTA: %c\n", (*textCompac)[k]);
	            		//exit(0);
	          		} 
	        	}
	      	}
	    }
	}

	printf("%s\n", (*textCompac));

	if(size%8!=0){
	    do{
	    	size++;
	        (*textCompac) = (char *)realloc((*textCompac), size+1*sizeof(char));
	        (*textCompac)[size]='0';
	    }while(size%8!=0);
	    for (i=size-8; i<=size; i++){
	        if((*textCompac)[i]=='1') count++;    
	    }
	    (*textCompac) = (char *)realloc((*textCompac), size+2*sizeof(char));
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
		size = (tabela->vet[j]->size_bin);
		//printf("TAMANHO DA LETRA: %d\n", size);
		aux = (char *)malloc(sizeof(char)*size);
		aux[size]='\0';
		for (i=0; i<size; i++){
			//printf("K: %d\n", k);
			aux[i] = textCompac[k];
			//printf("CHAR k: %c\n", textCompac[k]);
			//printf("CHAR: %c\n", aux[i]);
			k++;
		}
		if(strcmp(aux,tabela->vet[j]->bin)==0){
			textDescompac = (char *)realloc(textDescompac, (count+1)*sizeof(char));
			textDescompac[count]=tabela->vet[j]->letter;
			count++;
		}
		else{
			k=0;
			j++;	
		} 
	}while(textCompac[k]=='1' || textCompac[k]=='0');

	return textDescompac;
	
}

char* get_code(int code){
	char* bin = NULL;
	int n_bin = 0;
	//double n = (double)code;
	int n = code;
	int aux= 0;
	int frac;
	while(n>0){
		frac = n%10;
		n = n/10;
		/*printf("\n");
		n /= 10;
		frac = modf(n,&n);
		printf("frac = %.2lf\n", frac);
		frac *= 10;
		printf("frac = %.2lf\n", frac);
		printf("n = %.2lf\n", n);
		frac = fabs(frac);
		printf("frac = %.2lf\n", frac);
		*/
		if(frac == 1){
			bin = (char*)realloc(bin,sizeof(char)*(n_bin+1));
			bin[n_bin] = '0';
			n_bin++;
		}
		if(frac == 2){
			bin = (char*)realloc(bin,sizeof(char)*(n_bin+1));
			bin[n_bin] = '1';
			n_bin++;
		}
	}

	bin = (char*)realloc(bin,sizeof(n_bin+1));
	bin[n_bin] = '\0';

	return bin;
}

int is_valid(char* letters){
	if(letters[1]=='\0'){
		if(letters[0]>='A' && letters[0]<='Z') return TRUE;
		if(letters[0]>='a' && letters[0]<='z') return TRUE;
		if(letters[0]>='0' && letters[0]<='9') return TRUE;
		if(letters[0] == ' ') return TRUE;
	}
	return FALSE;
}

char* invert(char* code){
	int size = (int)strlen(code);
	char* new = (char*)malloc(sizeof(char)*(size+1));
	int i;
	for(i=0;i<size;i++){
		new[i] = code[size-i-1];
	}

	new[i] = '\0';
	return new;
}

void aux_fill_table(TABELA_HUFFMAN* tabela, NO* raiz, int* i, int code){

	if(is_valid(raiz->letters)){
		tabela->vet[*i] = create_element();
		tabela->vet[*i]->letter = raiz->letters[0];
		tabela->vet[*i]->bin = invert(get_code(code));
		tabela->vet[*i]->size_bin = (int)strlen(tabela->vet[*i]->bin);
		//printf("pegou code = %s\n", tabela->vet[*i]->bin);
		(*i)+=1;
	}
	else{
		if(raiz->heap_left != NULL)
			aux_fill_table(tabela,raiz->heap_left,i,code*10+1);
		if(raiz->heap_right != NULL)
			aux_fill_table(tabela,raiz->heap_right,i,code*10+2);
	}
}

void fill_table(TABELA_HUFFMAN* tabela,NO* raiz, int n_letters){

	int i = 0;

	tabela->n_elements = n_letters;
	tabela->vet = (ELEMENTS**)malloc(sizeof(ELEMENTS*)*n_letters);

	aux_fill_table(tabela, raiz, &i,0);

	return;
}