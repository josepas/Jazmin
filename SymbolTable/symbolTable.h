#ifndef SYMBTABLE
#define SYMBTABLE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/utils.h"
#include "../TypeTree/typeTree.h"

typedef struct _entry {
    char *string;
    int line;
    int column;
    struct _entry *next;
    struct _typetree *type;
} Entry;

typedef struct _symtable {
    int level;
    struct _symtable *father;
    struct _symtable *fchild, *lchild;
    struct _symtable *next;
    Entry **table;
} Symtable;



Symtable* createTable(Symtable*);
Symtable* enterScope(Symtable*);
Symtable* exitScope(Symtable*);
void dumpTable(Symtable*);
void destroyTable(Symtable*);
void insertTable(Symtable*, char*, int, int, struct _typetree*);
Entry* lookupTable(Symtable*, char*, int);

#endif