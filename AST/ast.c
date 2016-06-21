#include "ast.h"

AST* newAST() {
    
    AST* new = memory(sizeof(AST));

    new->tag = NULL;
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
        who->fist = nChild;
        who->last = nChild;
    } else {
        who->last->next = nChild;
        who->last = nChild;
    }

    return who;
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

AST* newCharNode(char a) {

    AST* node = newAST();
    node->tag = N_CHAR;

    node->u.a = a;

    return node;

}

AST* newCharNode(char a) {

    AST* node = newAST();
    node->tag = N_CHAR;

    node->u.a = a;

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