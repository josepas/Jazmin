#ifndef DEF_TAC
#define DEF_TAC
#define def_bin_op(X) newDLLNode(generateTAC(BIN_OP, X, ((Quad*)first->data)->result, ((Quad*)last->data)->result, genTemp()))
#define def_un_op(X) newDLLNode(generateTAC(UNARY_OP, X, ((Quad*)first->data)->result, NULL, genTemp()))
#define def_label(X) newDLLNode(generateTAC(TAC_LABEL, OP_LABEL, X, NULL, NULL))
#define def_asgn_array(X) newDLLNode(generateTAC(COPY_TO_INDEX, ASSIGN_TO_ARRAY, ((Quad*)first->data)->result, ((Quad*)last->data)->result, X))
#define def_asgn_fp(X) newDLLNode(generateTAC(TAC_FP, OP_TO_FP, ((Quad*)last->data)->result, NULL, X))
#define def_asgn(X,Y) newDLLNode(generateTAC(X, Y, ((Quad*)last->data)->result, NULL, ((Quad*)first->data)->result))
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
            TAC_REMOVE, TAC_EXIT , TAC_FP} TACType;

typedef enum {INT_PLUS, INT_MINUS, INT_MULT, INT_DIV, INT_MOD, INT_UN_MINUS,    //0..5
            FLOAT_PLUS, FLOAT_MINUS, FLOAT_MULT, FLOAT_DIV, FLOAT_MOD, FLOAT_UN_MINUS, //6..11
            GOTO, IF_GOTO, IFN_GOTO,    //12..14
            OP_EQUAL, OP_UNEQUAL, OP_LT, OP_LTOE, OP_GT, OP_GTOE,   //15..20
            ASSIGN, ASSIGN_FROM_ARRAY, ASSIGN_TO_ARRAY, //21..23
            ASSIGN_FROM_PTR, ASSIGN_TO_PTR, //24..25
            OP_LABEL, OP_COMMENT, OP_REMOVE, OP_EXIT,   //26..29
            OP_FROM_FP, OP_TO_FP} Operation;    //30..31

typedef enum {CONST_INT, CONST_FLOAT, CONST_CHAR, CONST_BOOL,
            LABEL, VAR, TEMP} AddrType;

typedef enum {L_VALUE, R_VALUE, NONE} LRValues;

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

Addr* genereateAddr(AddrType, void*);

Quad* generateTAC(TACType, Operation, Addr* , Addr*, Addr*);

void addrToString(Addr*, char*);

void imprimirTAC(Quad*);

Addr* genTemp(/*struct _symtable*, struct _typetree**/);

Addr* genLabel();

Node* astToTac(AST*, struct _dlinkedlist*, Addr*, Addr*, Addr*);

#endif