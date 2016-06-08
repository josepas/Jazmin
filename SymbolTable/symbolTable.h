#ifndef SYMBTABLE
#define SYMBTABLE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/utils.h"
#include "../TypeTree/typeTree.h"

// typedef enum {T_TYPE, T_VAR, T_FIELD, T_FUNC, T_PROC, T_POINTER, T_ARRAY, T_RECORD,
//                 T_TYPE, T_TYPE_ERROR, T_ANYTYPE} tsKind;

typedef enum {C_VAR, C_RECORD, C_SUB, C_CONSTANT, C_TYPE} Class;

typedef struct _entry {
    char *string;
    int line;
    int column;
    Class class;
    struct _typetree *type;
    struct _entry *next;
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
Symtable* enterScopeR(Symtable*, Symtable*);
Symtable* exitScope(Symtable*);

void dumpTable(Symtable*);
void destroyTable(Symtable*);
void insertTable(Symtable*, char*, int, int, Class, struct _typetree*);
Entry* lookupTable(Symtable*, char*, int);

#endif