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

AST* newReadNode(Entry *string, AST *var) {

    AST *node = newAST();
    node->tag = N_READ;

    addASTChild(node, var);
    
    // Read con string
    if (!string) {
        node->u.sym = string;
    }        

    return node;
}


AST* newBornNode(AST *var) {

    AST *node = newAST();
    node->tag = N_BORN;

    addASTChild(node, var);

    return node;

}

AST* newPuffNode(AST *var) {

    AST *node = newAST();
    node->tag = N_PUFF;

    addASTChild(node, var);

    return node;
}


// lo creo sin hijos porque no se cuantas condiciones puede tener
AST* newIfNode(AST* expr, AST* block, AST* eblock) {

    AST* node = newAST();
    node->tag = N_IF;

    addASTChild(node, expr);
    addASTChild(node, block);
    if (!eblock) {
        addASTChild(node, eblock);
    }

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


void dumpIfChildren(AST* who, int level) {
    if (who == NULL) {
        return;
    }

    if (who->next == NULL) {
        printf("else:\n");
        dumpAST(who, level + 1);
    }

    if (who->next != NULL) {
        printf("expr:\n");
        dumpAST(who, level + 1);
        printf("block:\n");
        dumpAST(who->next, level + 1);
    }

    return dumpIfChildren(who->next->next, level);

}

void dumpSeqChildren(AST* who, int level) {
    if (who == NULL) {
        return;
    }
    dumpAST(who, level);
    return dumpSeqChildren(who->next, level);
}

void dumpAST(AST* who, int level) {

    switch (who->tag) {
        case (N_IF) : {
            printf("IF:\n");

            dumpIfChildren(who->first, level);
            break;
        }
        case (N_WHILE) : {
            printf("WHILE:\n");

            printf("expr:\n");
            dumpAST(who->first, level + 1);

            printf("block:\n");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_BIN_OP) : {
            printf("BIN_EXPR:\n");

            printf("operator: %s.\n", who->operation);

            printf("1operand:\n");
            dumpAST(who->first, level + 1);

            printf("2operand:\n");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_UN_OP) : {
            printf("UN_EXPR:\n");
            printf("operator: %s.\n", who->operation);
            printf("operand:\n");
            dumpAST(who->first, level + 1);
            break;
        }
        case (N_INT) : {
            printf("INT: %d\n", who->u.i);
            break;
        }
        case (N_CHAR) : {
            printf("INT: %c\n", who->u.c);
            break;
        }
        case (N_BOOL) : {
            printf("BOOL:");
            if (who->u.b) {
                printf("False\n");
            } else {
                printf("True %d\n", who->u.b); // for debbuging debe ser 1
            }
            break;
        }
        case (N_VAR) : {
            printf("VAR: %s ", who->u.sym->string);
            dumpType(who->u.sym->type);
            printf("\n");
            break;
        }
        case (N_ASGN) : {
            printf("ASSIGN:\n");

            printf("operator: %s\n", who->operation);

            printf("l-value:\n");
            dumpAST(who->first, level + 1);

            printf("r-value:\n");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_FOR) : {
            printf("FOR:\n");

            printf("start:\n");
            dumpAST(who->first, level + 1);

            printf("end:\n");
            dumpAST(who->first->next, level + 1);
            
            if (who->first->next->next != who->last) {
                printf("step:\n");
                dumpAST(who->first->next->next, level + 1);
            }
            printf("block:\n");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_SEQ) : {
            printf("SEQUENCE:\n");
            dumpSeqChildren(who->first, level);         
            break;
        }
        case (N_WRITE) : {
            printf("WRITE:\n");
            if (who->u.sym) {
                dumpAST(who->first, level);
            } else {
                printf("string: %s\n", who->u.sym->string);
            }
            break;
        }
        case (N_READ) : {
            printf("READ:\n");
            dumpAST(who->first, level);
            if (!who->u.sym) {
                printf("string: %s\n", who->u.sym->string);
            }
            break;
        }
        case (N_BORN) : {
            printf("BORN:\n");
            dumpAST(who->first, level);
            break;
        }
        case (N_PUFF) : {
            printf("PUFF:\n");
            dumpAST(who->first, level);
            break;
        }
    }
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