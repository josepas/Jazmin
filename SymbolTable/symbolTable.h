#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

// typedef struct entrytag {
// 	char* table;
// 	int row;
// 	int column;
// } Entry;


typedef struct _symtable {
	struct _symtable *father;
	struct _symtable *fchild, *lchild;
	struct _symtable *next;
	Hash *table;
    int level;
} Symtable;



Symtable* createTable(Symtable*);
void enterScope(Symtable**);
void exitScope(Symtable**);
void dumpTable(Symtable*);
void destroyTable(Symtable*);
void insertTable(Symtable*, char*, int, int);