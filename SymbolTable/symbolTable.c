#include "symbolTable.h"


Symtable* createTable(Symtable *father) {

	Symtable* newTable;

	newTable = (Symtable*) memory(sizeof(Symtable));
	newTable->table = createHash(17);
	newTable->level = 0;
	newTable->father = father;
	newTable->fchild = NULL;
	newTable->lchild = NULL;
	newTable->next  = NULL;

	return newTable;
}


void enterScope(Symtable **current) {

	Symtable *nTable = createTable(*current);

	nTable->level = (*current)->level + 1;

	if (*current == NULL) {
		*current = nTable;
	} else if ( (*current)->fchild == NULL ) {
		(*current)->fchild = nTable;
		(*current)->lchild = nTable;
	} else {
		(*current)->lchild->next = nTable;
		(*current)->lchild = nTable;
	}

	*current = nTable;
	return;
}


void exitScope(Symtable **current) {
	*current = (*current)->father;
}


void dumpTable(Symtable *printer) {
	dumpHash(printer->table, printer->level);

	if (printer->fchild != NULL) {
		dumpTable(printer->fchild);
	}
	if (printer->next != NULL) {
		dumpTable(printer->next);
	}

}


void destroyTable(Symtable *who) {

	if ( who->fchild != NULL ) {
		destroyTable(who->fchild);
	}
	if ( who->next != NULL ) {
		destroyTable(who->next);
	}

	destroyHash(who->table);

	free(who);
}


void insertTable(Symtable *current, char *str, int line, int column) {
	insertHash(current->table, str, line, column);
}
