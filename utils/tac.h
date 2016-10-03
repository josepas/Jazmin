#ifndef DEF_TAC
#define DEF_TAC
#define def_op(X) newDLLNode(generateTAC(BIN_OP, X, ((Quad*)list->last->prev->data)->result, ((Quad*)list->last->data)->result, genTemp()))
#include <stdio.h>
#include "../TypeTree/typeTree.h"
#include "../SymbolTable/symbolTable.h"
#include "../AST/ast.h"
#include "../List/DoublyLinkedList.h"
#include "utils.h"

typedef enum {BIN_OP, UNARY_OP, COPY, JUMP, IF_JUMP, IFN_JUMP, IF_RELOP_JUMP,
            PARAM, PROC_CALL, FUNC_CALL, TAC_RETURN, RETURN_VALUE,
            COPY_FROM_INDEX, COPY_TO_INDEX, COPY_ADDRESS, COPY_VALUE_POINTED,
            COPY_TO_POINTED, TAC_READ, PRINT, TAC_COMMENT, TAC_LABEL,
            TAC_REMOVE } TACType;

typedef enum {INT_PLUS, INT_MINUS, INT_MULT, INT_DIV, INT_MOD,
            FLOAT_PLUS, FLOAT_MINUS, FLOAT_MULT, FLOAT_DIV, FLOAT_MOD,
            GOTO, IF_GOTO, IFN_GOTO,
            OP_EQUAL, OP_UNEQUAL, OP_LT, OP_LTOE, OP_GT, OP_GTOE,
            ASSIGN, ASSIGN_FROM_ARRAY, ASSIGN_TO_ARRAY,
            ASSIGN_FROM_PTR, ASSIGN_TO_PTR,
            OP_LABEL, OP_COMMENT, OP_REMOVE } Operation;

typedef enum {CONST_INT, CONST_FLOAT, CONST_CHAR,
            LABEL, VAR, TEMP} AddrType;

typedef struct _addr {
    AddrType addt;
    union {
        // Literales
        int i;
        float f;
        char c;
        // Variables
        Entry *e;
        // Temporales
        unsigned int t;
    } u;
} Addr;

typedef struct _quad {
    Operation op;
    Addr *arg1;
    Addr *arg2;
    Addr *result;
} Quad;

Quad* generateTAC(TACType, Operation, Addr* , Addr*, Addr*);

void addrToString(Addr*, char*);

void imprimirTAC(Quad*);

Addr* genTemp(/*struct _symtable*, struct _typetree**/);

void astToTac(AST*, struct _dlinkedlist*);

#endif