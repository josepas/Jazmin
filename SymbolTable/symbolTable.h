#include <stdlib.h>
#include <stdio.h>
#include "hash.h"


typedef struct _symtable {
	struct _symtable *father;
	struct _symtable *fchild, *lchild;
	struct _symtable *next;
	Hash *table;
    int level;
} Symtable;



Symtable* createTable(Symtable*);
Symtable* enterScope(Symtable*);
Symtable* exitScope(Symtable*);
void dumpTable(Symtable*);
void destroyTable(Symtable*);
void insertTable(Symtable*, char*, int, int);
Entry* lookupTable(Symtable*, char*, int);