#ifndef UTILS
#define UTILS
#include <stdio.h>
#include <stdlib.h>
#include "../TypeTree/typeTree.h"

typedef struct _argList ArgList;

typedef struct _typeNode {
    struct _typetree *t;
    struct _typeNode *next;
} TypeNode;

struct _argList {
    TypeNode *f;
    TypeNode *l;
};

void* memory(size_t);

ArgList* newArgList(struct _typetree*);

ArgList* add(ArgList*, struct _typetree*);

#endif