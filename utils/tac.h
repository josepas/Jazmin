#ifndef DEF_TAC
#define DEF_TAC
#define def_bin_op(X) newDLLNode(generateTAC(BIN_OP, X, ((Quad*)first->data)->result, ((Quad*)last->data)->result, genTemp()))
#define def_un_op(X) newDLLNode(generateTAC(UNARY_OP, X, ((Quad*)first->data)->result, NULL, genTemp()))
#define def_label(X) newDLLNode(generateTAC(TAC_LABEL, OP_LABEL, X, NULL, NULL))
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

typedef enum {INT_PLUS, INT_MINUS, INT_MULT, INT_DIV, INT_MOD, INT_UN_MINUS,
            FLOAT_PLUS, FLOAT_MINUS, FLOAT_MULT, FLOAT_DIV, FLOAT_MOD, FLOAT_UN_MINUS,
            GOTO, IF_GOTO, IFN_GOTO,
            OP_EQUAL, OP_UNEQUAL, OP_LT, OP_LTOE, OP_GT, OP_GTOE,
            ASSIGN, ASSIGN_FROM_ARRAY, ASSIGN_TO_ARRAY,
            ASSIGN_FROM_PTR, ASSIGN_TO_PTR,
            OP_LABEL, OP_COMMENT, OP_REMOVE } Operation;

typedef enum {CONST_INT, CONST_FLOAT, CONST_CHAR, CONST_BOOL,
            LABEL, VAR, TEMP} AddrType;

typedef struct _addr {
    AddrType addt;
    union {
        // Literales
        int i;
        float f;
        char c;
        int b;
        // Variables
        Entry *e;
        // Temporales
        unsigned int t;
        // Label
        unsigned int l;
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

Addr* genLabel();

Node* astToTac(AST*, struct _dlinkedlist*, Addr*, Addr*);

#endif