#include "generator.h"
// #include "genWrapper.h"


void generateMIPS(DLinkedList* list) {

	for (Node* aux = list->first; aux != NULL; aux = aux->next) {
		imprimirTAC( (Quad*)aux->data);
	}



}


