#include "symbolTable.h"


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
	return;
}


void exitScope(Symtable *current) {
	current = current->father;
}


void dumpTable(Symtable *printer) {
	// dumpHash(printer->table);

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
	//if (who == NULL) no se si esto va, pendiente
	free(who);
	return;
}
