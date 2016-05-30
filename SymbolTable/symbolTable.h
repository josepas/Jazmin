#include <stdlib.h>
#include <stdio.h>


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
void insertTable(Symtable*, char*, int, int);
Entry* lookupTable(Symtable*, char*, int);

