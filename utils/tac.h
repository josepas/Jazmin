#ifndef TAC
#define TAC
#include "utils.h"
#include "../SymbolTable/symbolTable.h"
#include "../TypeTree/typeTree.h"
#include <stdio.h>

typedef enum {BIN_OP, UNARY_OP, COPY, JUMP, IF_JUMP, IFN_JUMP, IF_RELOP_JUMP,
            PARAM, PROC_CALL, FUNC_CALL, RETURN, RETURN_VALUE,
            COPY_FROM_INDEX, COPY_TO_INDEX, COPY_ADDRESS, COPY_VALUE_POINTED,
            COPY_TO_POINTED, READ, PRINT, COMMENT } TACType;

typedef enum { INT_PLUS, INT_MINUS, INT_MULT, INT_DIV, INT_MOD,
            FLOAT_PLUS, FLOAT_MINUS, FLOAT_MULT, FLOAT_DIV, FLOAT_MOD,
            GOTO, IF_GOTO, IFN_GOTO,
            EQUAL, UNEQUAL, LT, LTOE, GT, GTOE,
            ASSIGN } Operation;

typedef struct _quad {
    Operation op;
    void *arg1;
    void *arg2;
    void *result;
} Quad;

int t_num;

Quad* generateTAC(TACType, Operation, void* , void*, void*);

void imprimirTAC(Quad*);

Entry* genTemp(struct _symtable*, struct _typetree*);

#endif