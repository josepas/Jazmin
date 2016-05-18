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


Symtable* enterScope(Symtable *current) {

	Symtable *nTable = createTable(current);

	if (current == NULL) {
		current = nTable;
		nTable->level = 0;
	} else if ( current->fchild == NULL ) {
		nTable->level = current->level + 1;
		current->fchild = nTable;
		current->lchild = nTable;
	} else {
		nTable->level = current->level + 1;
		current->lchild->next = nTable;
		current->lchild = nTable;
	}

	return nTable;
}


Symtable* exitScope(Symtable *current) {
	return current->father;
}


void dumpTable(Symtable *printer) {
	printf("%*sScope %d:\n", printer->level*3, "", printer->level);
	dumpHash(printer->table, printer->level);
	printf("========================================\n");

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
	if(lookupHash(current->table, str) == NULL) {
		insertHash(current->table, str, line, column);
	}
	else {
		fprintf(stderr, "Error:%d:%d: Redeclaración %s\n", line, column, str);
	}
}

Entry* lookupTable(Symtable* current, char* key, int local) {
	if(current == NULL) {
		return NULL;
	}

	Entry* symbol = lookupHash(current->table, key);

	if(symbol == NULL && local == 0) {
		return lookupTable(current->father, key, local);
	}

	return symbol;
}
