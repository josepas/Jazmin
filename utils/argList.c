#include "../Typetree/typetree.h"
#include "utils.h"


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