#ifndef TYPETREE
#define TYPETREE
#include "../SymbolTable/symbolTable.h"

typedef enum {T_INT, T_FLOAT, T_BOOL, T_CHAR, T_HOLLOW,
                T_FUNC, T_POINTER, T_CONF, T_STRUCT, T_TYPE_ERROR} Kind;

typedef struct _typetree Typetree;


struct _typetree {
    Kind kind;
    int size;
    union {
        struct {
            int low;
            int high;
            Typetree *t;

        } a;
        struct {
            Typetree *t;
        } p;
        struct {
            Typetree *f;
            Typetree *s;
        } t;
        struct {
            Typetree *dom; // aqui creo que deberian entrar las tuplas
            Typetree *range;
        } fun;
        struct {
            struct _symtable *campos;
        } r;
    } u;

};

#endif