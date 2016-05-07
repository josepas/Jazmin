#include <stdlib.h>
#include <stdio.h>

#include "symbolTable.h"


void* memory(size_t type) {

	void *ptr = malloc(type);
	if (ptr == NULL) {
		fprintf(stderr, "Fatal: Error en alocacion de memoria\n");
	}
	return ptr;
}

Symtable* createTable(Symtable *father) {
	
	Symtable* newTable;
	
	newTable = (Symtable*) memory(sizeof(Symtable));
	newTable->father = father;
	newTable->fchild = NULL;
	newTable->lchild = NULL;
	newTable->next  = NULL;

	return newTable;
}

void enterScope(Symtable *current) {
	
	Symtable *nTable = createTable(current);
	
	if (current == NULL) {
		current = nTable;
	} else if ( current->fchild == NULL ) {
		current->fchild = nTable;
		current->lchild = nTable;
	} else {
		current->lchild->next = nTable;
		current->lchild = nTable;
	}

	current = nTable;
}

void exitScope(Symtable *current) {
	current = current->father;
}


int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}
