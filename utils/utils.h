#ifndef UTILS
#define UTILS
#include <stdio.h>
#include <stdlib.h>
#include "../TypeTree/typeTree.h"


void* memory(size_t);

typedef struct _offsetStack {
    int offset;
    struct _offsetStack *prev;
} Offsetstack;

Offsetstack* createStack();
void push(Offsetstack*, int);
int pop(Offsetstack*);

#endif