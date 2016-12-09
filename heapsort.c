#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

#define TOP 0
#define get_parent(id) (id-1)/2
#define get_left_child(id) (id*2)+1
#define get_right_child(id) (id*2)+2


void swap(NO **vector, int i, int j) {
	NO* aux = vector[i];
	vector[i] = vector[j];
	vector[j] = aux;
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
void correctUp(NO** vector,int index)
	{
		// Se index não é a raiz e o valor do index for menor do que o valor de seu pai,
		// troca seus valores (index e pai(index)) e corrige para o pai
		if (index > 1 && vector[index]->prob < vector[get_parent(index)]->prob)
		{
			swap(vector,index,get_parent(index));
			correctUp(vector,get_parent(index));
		}
	}

void max_heapify(NO **vector, int n, int index) {

	int left_id = get_left_child(index);	 
	int right_id = get_right_child(index);   
	int largest_id = index;			 

	if (left_id < n && vector[left_id]->prob > vector[largest_id]->prob) 
		largest_id = left_id;
	if (right_id < n && vector[right_id]->prob > vector[largest_id]->prob) 
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

void heapsort_(NO **vector, int n) {
	int i;
	
	build_max_heap(vector, n);

	for (i = 1; i < n; i++) {
		swap(vector, TOP, n-i);
		max_heapify(vector, n-i, TOP);
	}
}








