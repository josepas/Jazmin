#ifndef DEF_AST
#define DEF_AST
#include "../utils/utils.h"


typedef enum {N_IF, N_WHILE, N_BIN_OP, N_UN_OP,
     N_INT, N_CHAR, N_BOOL, N_VAR, N_ASGN, N_FOR, N_SEQ, N_WRITE, N_READ, N_BORN, N_PUFF} NodeTag;

typedef struct _ast {
    NodeTag tag;

    char *operation;

    Typetree *type;

    struct _ast *first;
    struct _ast *last;
    struct _ast *next;

    union {
        struct _entry *sym;
        int i;
        int b;
        float f;
        char c;
    } u;

} AST;


AST* newAST();
AST* addASTChild (AST*, AST*);
AST* newSeqNode();
AST* newIfNode(AST*, AST*, AST*);

AST* newUnaryOp(char*, AST*);
AST* newBinOp(AST*, char*, AST*);
AST* newAssignNode(AST*, char*, AST*);

AST* newBornNode(AST*);
AST* newPuffNode(AST*); 
AST* newWhileNode(AST* , AST* );
AST* newForNode(AST*, AST*, AST*, AST*);

AST* newWriteNode(Entry*, AST*);
AST* newReadNode(Entry*, AST*);

AST* newVarNode(Entry*);
AST* newIntNode(int);
AST* newCharNode(char);
AST* newBoolNode(int);

void dumpAST(AST*, int);
void destroyAST(AST*);

#endif