#include "typeTree.h"


Typetree *createType(Kind kind) {

    Typetree *type = (Typetree*) memory(sizeof(Typetree));

    type->kind = kind;
    type->size = 1; //CAMBIARLO

    return type;
}

Typetree* createArray(int n, Typetree *type) {
    Typetree *new_type = (Typetree*) memory(sizeof(Typetree));

    new_type->kind = T_ARRAY;
    new_type->size = 1; //CAMBIARLO

    new_type->u.a.low = 0;
    new_type->u.a.high = n-1;
    new_type->u.a.t = type;

    return new_type;
}

Typetree* createProc(ArgList *list) {
    Typetree *new_type = (Typetree*) memory(sizeof(Typetree));

    new_type->kind = T_PROC;
    new_type->size = 1; //CAMBIARLO

    new_type->u.proc.dom = list;

    return new_type;
}

void dumpType(Typetree *type) {
    switch(type->kind) {
        case T_INT :
            printf("int\n");
            break;
        case T_CHAR :
            printf("char\n");
            break;
        case T_FLOAT :
            printf("float\n");
            break;
        case T_POINTER :
            printf("pointer to ");
            dumpType(type->u.p.t);
            break;
        case T_ARRAY :
            printf("array 0..%d of ", type->u.a.high);
            dumpType(type->u.a.t);
            break;
        default :
            printf("noooooo %d\n", type->kind);
            break;
    }
}