#ifndef DEF_AST
#define DEF_AST
#include "../utils/utils.h"


typedef enum {N_ERROR, N_IF, N_WHILE, N_BIN_OP, N_UN_OP,
     N_INT, N_FLOAT, N_CHAR, N_BOOL, N_VAR, N_ASGN, N_FOR, N_SEQ,
     N_WRITE, N_READ, N_BORN, N_PUFF, N_PTR,
    N_RETURN, N_NEXT, N_BREAK, N_FUNC, N_FCALL, N_PCALL, N_PROGRAM} NodeTag;


typedef struct _ast {
    NodeTag tag;

    char *operation;

    Typetree *type;

    struct _ast *first;
    struct _ast *last;
    struct _ast *next;

    union {
        struct _entry *sym;
        struct _symtable *tabla;
        int i;
        int b;
        float f;
        char c;
    } u;

} AST;


AST* newAST();
AST* addASTChild (AST*, AST*);
AST* newSeqNode(Symtable*);
AST* newIfNode(AST*, AST*, AST*, AST*);

AST* newUnaryOp(char*, AST*);
AST* newBinOp(AST*, char*, AST*);
AST* newAssignNode(AST*, char*, AST*);


AST* newBornNode(AST*);
AST* newPuffNode(AST*);
AST* newWhileNode(AST* , AST* );
AST* newForNode(AST*, AST*, AST*, AST*);

AST* newBornNode(AST*);
AST* newPuffNode(AST*);
AST* newPuffNode(AST*);

AST* newWriteNode(Entry*, AST*);
AST* newReadNode(Entry*, AST*);

AST* newReturnNode(AST*);
AST* newNextNode();
AST* newBreakNode();

AST* newVarNode(Entry*);
AST* newPtrNode(Entry*);
AST* newIntNode(int);
AST* newFloatNode(float);
AST* newCharNode(char);
AST* newBoolNode(int);

void dumpVar(AST*);
void dumpAST(AST*, int);
AST* newErrorNode(Typetree*);

void destroyAST(AST*);

AST* newBaseTypeNode(Kind);
AST* newFuncNode(Entry*, AST*);
AST* newFunCallNode(Entry*);
AST* newProcCallNode(Entry*);
AST* newSubCallNode(Entry*);
AST* newProgramNode(AST*, AST*);

#endif