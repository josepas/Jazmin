#include "ast.h"

AST* newAST() {

    AST* new = memory(sizeof(AST));

    new->tag = -1;
    new->operation = NULL;
    new->first = NULL;
    new->last = NULL;
    new->next = NULL;

    return new;
}


AST* addASTChild (AST *who, AST *nChild) {

    if (who == NULL) {
        fprintf(stderr, "Fatal: Creacion de arbol\n");
    }
    // No tenia ningun hijo
    if (who->first == NULL) {
        who->first = nChild;
        who->last = nChild;
    } else {
        who->last->next = nChild;
        who->last = nChild;
    }

    return who;
}

AST* newSeqNode() {

    AST* node = newAST();
    node->tag = N_SEQ;

    return node;
}


AST* newWriteNode(Entry *string, AST *var) {

    AST *node = newAST();
    node->tag = N_WRITE;

    if (string && var) {
        fprintf(stderr, "Fatal: Error en la creacion del arbol\n");
    }

    // si no es con var (string)
    if (var) {
        node->u.sym = string;
    }

    // si no es con string (var)
    if (string) {
        addASTChild(node, var);
    }

    return node;
}

// lo creo sin hijos porque no se cuantas condiciones puede tener
AST* newIfNode() {

    AST* node = newAST();
    node->tag = N_IF;

    return node;
}

AST* newUnaryOp(char *op, AST *expr1) {

    AST* node = newAST();
    node->tag = N_UN_OP;
    node->operation = op;

    addASTChild(node, expr1);

    return node;

}


AST* newBinOp(AST *expr1, char *op, AST *expr2) {

    AST* node = newAST();
    node->tag = N_BIN_OP;
    node->operation = op;

    addASTChild(node, expr1);
    addASTChild(node, expr2);

    return node;
}


AST* newWhileNode(AST* expr, AST* block) {

    AST* node = newAST();
    node->tag = N_WHILE;

    addASTChild(node, expr);
    addASTChild(node, block);

    return node;
}

AST* newForNode(AST *start, AST *end, AST *step, AST *block) {

    AST *node = newAST();
    node->tag = N_FOR;

    addASTChild(node, start);
    addASTChild(node, end);
    if (!step) {
        addASTChild(node, step);
    }
    addASTChild(node, block);

    return node;
}


AST* newAssignNode(AST *var, char *op, AST *expr) {

    AST* node = newAST();
    node->tag = N_ASGN;
    node->operation = op;

    addASTChild(node, var);
    addASTChild(node, expr);

    return node;
}


AST* newVarNode(Entry *sym) {

    AST* node = newAST();
    node->tag = N_VAR;

    node->u.sym = sym;

    return node;
}


AST* newIntNode(int x) {

    AST* node = newAST();
    node->tag = N_INT;

    node->u.i = x;

    return node;
}

AST* newCharNode(char c) {

    AST* node = newAST();
    node->tag = N_CHAR;

    node->u.c = c;

    return node;

}

AST* newBoolNode(int b) {

    AST* node = newAST();
    node->tag = N_BOOL;

    node->u.b = b;

    return node;
}

void destroyAST(AST* who) {

    if (who == NULL) {
        return;
    }
    if ( who->first != NULL ) {
        destroyAST(who->first);
    }
    if ( who->next != NULL ) {
        destroyAST(who->next);
    }

    // quizas haya que destruir mas cosas aqui en el futuro
    free(who);
}