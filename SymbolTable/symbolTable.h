#ifndef SYMBTABLE
#define SYMBTABLE
#define HASH_SIZE 17
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/utils.h"
#include "../TypeTree/typeTree.h"


typedef enum {C_VAR, C_ARRAY, C_RECORD, C_SUB, C_CONSTANT, C_TYPE} Class;

typedef enum {GLOBAL, LOCAL} Scope;

typedef struct _entry {
    char *string;
    int line;
    int column;
    Class class;
    Scope scope;
    struct _typetree *type;
    int size;
    int offset;
    struct _ast *ast;

    struct _entry *next;
} Entry;

typedef struct _symtable {
    int level;
    int size;
    struct _symtable *father;
    struct _symtable *fchild, *lchild;
    struct _symtable *next;
    Entry **table;
} Symtable;

Symtable* createTable(Symtable*);
Symtable* enterScope(Symtable*);
Symtable* enterScopeR(Symtable*, Symtable*);
Symtable* exitScope(Symtable*);

void dumpTable(Symtable*);
void destroyTable(Symtable*);
void insertTable(Symtable*, char*, int, int, Class, struct _typetree*, int, int);
Entry* lookupTable(Symtable*, char*, int);

int getMaxSize(Symtable*);

#endif