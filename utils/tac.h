#ifndef DEF_TAC
#define DEF_TAC
#define def_bin_op(X) newDLLNode(generateTAC(BIN_OP, X, ((Quad*)first->data)->result, ((Quad*)last->data)->result, genTemp()))
#define def_un_op(X) newDLLNode(generateTAC(UNARY_OP, X, ((Quad*)first->data)->result, NULL, genTemp()))
#define def_label(X) newDLLNode(generateTAC(TAC_LABEL, OP_LABEL, X, NULL, NULL))
#define def_asgn_array(X) newDLLNode(generateTAC(COPY_TO_INDEX, ASSIGN_TO_ARRAY, ((Quad*)first->data)->result, ((Quad*)last->data)->result, X))
#define def_asgn_fp(X) newDLLNode(generateTAC(TAC_FP, OP_TO_FP, ((Quad*)last->data)->result, NULL, X))
#define def_asgn(X,Y) newDLLNode(generateTAC(X, Y, ((Quad*)last->data)->result, NULL, ((Quad*)first->data)->result))
#define def_exit() newDLLNode(generateTAC(TAC_EXIT, OP_EXIT, NULL, NULL, NULL))
#define def_goto(X) newDLLNode(generateTAC(JUMP, GOTO, NULL, NULL, X))
#define def_comment() newDLLNode(generateTAC(TAC_COMMENT, OP_COMMENT, NULL, NULL, NULL))
#define def_logue(X, Y) newDLLNode(generateTAC(PRO_EPI_LOGUE, X, Y, NULL, NULL))
#define def_write(X) newDLLNode(generateTAC(TAC_WRITE,  X, ((Quad*)temp->data)->result, NULL, NULL))
#include <stdio.h>
#include "../TypeTree/typeTree.h"
#include "../SymbolTable/symbolTable.h"
#include "../AST/ast.h"
#include "../List/DoublyLinkedList.h"
#include "utils.h"

typedef enum {BIN_OP, UNARY_OP, COPY, JUMP, IF_JUMP, IFN_JUMP, IF_RELOP_JUMP, //0..6
            PARAM, PROC_CALL, TAC_FUNC_CALL, TAC_RETURN, TAC_RETURN_VALUE,  //7..11
            COPY_FROM_INDEX, COPY_TO_INDEX, COPY_ADDRESS, COPY_VALUE_POINTED,   //12..
            COPY_TO_POINTED, TAC_READ, TAC_WRITE, TAC_COMMENT, TAC_LABEL,
            TAC_REMOVE, TAC_EXIT , TAC_FP, TAC_GP,
            PRO_EPI_LOGUE} TACType;

typedef enum {INT_PLUS, INT_MINUS, INT_MULT, INT_DIV, INT_MOD, INT_UN_MINUS,    //0..5
            FLOAT_PLUS, FLOAT_MINUS, FLOAT_MULT, FLOAT_DIV, FLOAT_MOD, FLOAT_UN_MINUS, //6..11
            GOTO, IF_GOTO, IFN_GOTO,    //12..14
            OP_EQUAL, OP_UNEQUAL, OP_LT, OP_LTOE, OP_GT, OP_GTOE,   //15..20
            ASSIGN, ASSIGN_FROM_ARRAY, ASSIGN_TO_ARRAY, //21..23
            ASSIGN_FROM_PTR, ASSIGN_TO_PTR, //24..25
            OP_LABEL, OP_COMMENT, OP_REMOVE, OP_EXIT,   //26..29
            OP_FROM_FP, OP_TO_FP, OP_FROM_GP, OP_TO_GP,
            OP_PARAM, OP_FUNC_CALL, OP_FUNC_DEF,
            OP_RETURN, OP_RETURN_VALUE,
            PROLOGUE, EPILOGUE,
            OP_WRITE_INT, OP_WRITE_FLOAT, OP_WRITE_CHAR, OP_WRITE_BOOL,
            OP_WRITE_STR } Operation;

typedef enum {CONST_INT, CONST_FLOAT, CONST_CHAR, CONST_BOOL, CONST_STR,
            LABEL, VAR, TEMP, SUBROUTINE} AddrType;

typedef enum {L_VALUE, R_VALUE, NONE} LRValues;

typedef enum {E, B, N} Context;

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
        // Nombre funcion
        char *f_name;
        // string
        char *str;
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

Node* generateNodeVar(struct _dlinkedlist*, AST*, Scope, Addr*);

void addrToString(Addr*, char*);

void imprimirTAC(Quad*);

Addr* genTemp(/*struct _symtable*, struct _typetree**/);

Addr* genLabel();

Node* astToTac(AST*, struct _dlinkedlist*, Addr*, Addr*, Addr*, Context, Addr*);

void cleanTAC(struct _dlinkedlist*);

int funcParamsTAC(AST*, DLinkedList*, Addr*, Addr*, Addr*, Context, Addr*);

#endif