#include "typeTree.h"


Typetree *createType(Kind kind) {

    Typetree *type = (Typetree*) memory(sizeof(Typetree));

    type->kind = kind;

    switch(kind) {
        case T_HOLLOW :
            type->size = 0;
            break;
        case T_INT :
            type->size = 4;
            break;
        case T_CHAR :
            type->size = 4;
            break;
        case T_FLOAT :
            type->size = 4;
            break;
        case T_BOOL :
            type->size = 4;
            break;
        case T_POINTER :
            type->size = 4;
            break;
    }

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

Typetree* createStruct(char *name) {
    Typetree *new_type = (Typetree*) memory(sizeof(Typetree));

    new_type->kind = T_STRUCT;
    new_type->size = 1; //CAMBIARLO

    new_type->u.r.name = name;

    return new_type;
}

Typetree* createConf(char *name) {
    Typetree *new_type = (Typetree*) memory(sizeof(Typetree));

    new_type->kind = T_CONF;
    new_type->size = 1; //CAMBIARLO

    new_type->u.r.name = name;

    return new_type;
}

Typetree* createProc(ArgList *list) {
    Typetree *new_type = (Typetree*) memory(sizeof(Typetree));

    new_type->kind = T_PROC;
    new_type->size = 1; //CAMBIARLO

    new_type->u.proc.dom = list;


    return new_type;
}

Typetree* createFunc(ArgList *list, Typetree *range) {
    Typetree *new_type = (Typetree*) memory(sizeof(Typetree));

    new_type->kind = T_FUNC;
    new_type->size = 1; //CAMBIARLO

    new_type->u.fun.dom = list;
    new_type->u.fun.range = range;

    return new_type;
}



ArgList* newArgList() {

    ArgList *newL;
    newL = (ArgList*) memory( sizeof(ArgList) );

    newL->f = NULL;
    newL->l = NULL;

    return newL;
}

ArgList* add(ArgList* l, Typetree* t) {
    TypeNode *newN;
    newN = (TypeNode*) memory( sizeof(TypeNode) );
    newN->t = t;
    newN->next = NULL;

    if (l->f == NULL) { //vacia
        l->f = newN;
        l->l = newN;
    } else {
        l->l->next = newN;
        l->l = newN;
    }

    return l;
}

int compareTypeNodes(TypeNode *n1, TypeNode *n2) {

    //Llegue al final
    if (n1 == NULL && n2 == NULL) {
        return 1;
    }
    //alguno es nulo
    if (n1 == NULL || n2 == NULL) {
        return 0;
    }
    // son distintos
    if (n1->t != n2->t) {
        return 0;
    }

    return compareTypeNodes(n1->next, n2->next);
}

void dumpArgList(ArgList* who) {

    if (who == NULL) {
        // aqui deberia ser assert
        printf("Error creando la lista de formales\n");
        return;
    }
    if (who->f == NULL) {
        printf("Vacio");
    }
    TypeNode *printer = who->f;
    while (printer != NULL) {
        dumpType(printer->t);
        printf(" X ");
        printer = printer->next;
    }
}

void dumpType(Typetree *type) {
    switch(type->kind) {
        case T_INT :
            printf("int");
            break;
        case T_CHAR :
            printf("char");
            break;
        case T_FLOAT :
            printf("float");
            break;
        case T_BOOL :
            printf("bool");
            break;
        case T_HOLLOW :
            printf("hollow");
            break;
        case T_POINTER :
            printf("pointer to ");
            dumpType(type->u.p.t);
            break;
        case T_ARRAY :
            printf("array 0..%d of ", type->u.a.high);
            dumpType(type->u.a.t);
            break;
        case T_FUNC :
            printf("Function: ");
            dumpArgList(type->u.fun.dom);
            printf("-> ");
            dumpType(type->u.fun.range);
            break;
        case T_PROC :
            printf("Procedure: ");
            dumpArgList(type->u.fun.dom);
            // dumpType(type->u.fun.range); se le pondra hollow?
            break;
        case T_CONF :
            printf("Confederation %s", type->u.r.name);
            break;
        case T_STRUCT :
            printf("Struct %s", type->u.r.name);
            break;
        default :
            printf("Sin definir %d\n", type->kind);
            break;
    }
}