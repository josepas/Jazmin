#include "../Typetree/typetree.h"


typedef struct _typeNode {
    Typetree *t;
    struct _typeNode *next;
} TypeNode;

typedef struct _argList {
    TypeNode *f;
    TypeNode *l;    
} ArgList;

void* memory(size_t type) {

    void *ptr = malloc(type);
    if (ptr == NULL) {
        fprintf(stderr, "Fatal: Error en alocacion de memoria\n");
    }
    return ptr;
}


ArgList* newArgList(Typetree* t) {
    
    ArgList *newL;
    newL = (ArgList*) memory( sizeof(ArgList) );
    
    TypeNode *newN;
    newN = (TypeNode*) memory( sizeof(TypeNode));
    newN->t = t;
    newN->next = NULL;

    newL->f = newN;
    newL->l = newN;

    return newL;
}

ArgList* add(ArgList* l, Typetree* t) {

    TypeNode *newN;
    newN = (TypeNode*) memory( sizeof(TypeNode) );
    newN->t = t;
    newN->next = NULL;

    l->l.next = newN;
    l->l = newN;

    return l;
}