#include "ast.h"

extern Symtable *current;

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
    if (var == NULL) {
        node->u.sym = string;
    }

    // si no es con string (var)
    if (string == NULL) {
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
AST* newIfNode(AST* expr, AST* block, AST* elifblock, AST* eblock) {

    AST* node = newAST();
    node->tag = N_IF;

    addASTChild(node, expr);
    addASTChild(node, block);
    if (elifblock) {
        addASTChild(node, elifblock->first);
        node->last = elifblock->last;
        free(elifblock);
    }
    if (eblock) {
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
    if (step) {
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


AST* newReturnNode(AST *expr) {

    AST *node = newAST();
    node->tag = N_RETURN;

    if (expr) {
        addASTChild(node, expr);
    }

    return node;
}

AST* newNextNode() {

    AST* node = newAST();
    node->tag = N_NEXT;

    return node;
}

AST* newBreakNode() {

    AST* node = newAST();
    node->tag = N_BREAK;

    return node;
}


AST* newPtrNode(Entry* e) {

    AST* node = newAST();
    node->tag = N_PTR;

    node->type = e->type;
    return node;
}


AST* newVarNode(Entry *sym) {

    AST* node = newAST();
    node->tag = N_VAR;

    node->u.sym = sym;
    node->type = sym->type;

    return node;
}


AST* newIntNode(int x) {

    AST* node = newAST();
    node->tag = N_INT;

    node->u.i = x;
    node->type = lookupTable(current, "int", 0)->type;

    return node;
}

AST* newFloatNode(float f) {

    AST* node = newAST();
    node->tag = N_FLOAT;

    node->u.f = f;
    node->type = lookupTable(current, "float", 0)->type;

    return node;
}

AST* newCharNode(char c) {

    AST* node = newAST();
    node->tag = N_CHAR;

    node->u.c = c;
    node->type = lookupTable(current, "char", 0)->type;

    return node;

}


AST* newBoolNode(int b) {

    AST* node = newAST();
    node->tag = N_BOOL;

    node->u.b = b;
    node->type = lookupTable(current, "bool", 0)->type;

    return node;
}


void dumpIfChildren(AST* who, int level) {
    if (who == NULL) {
        return;
    }

    if (who->next == NULL) {
        printf("%*selse:\n", level*3, " ");
        dumpAST(who, level + 1);
        return;
    }

    if (who->next != NULL) {
        printf("%*sexpr:\n", level*3, " ");
        dumpAST(who, level + 1);
        printf("%*sblock:\n", level*3, " ");
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
            printf("%*sIF:\n", level*3, " ");

            dumpIfChildren(who->first, level);
            break;
        }
        case (N_WHILE) : {
            printf("%*sWHILE:\n", level*3, " ");
            printf("%*sexpr:\n", level*3, " ");
            dumpAST(who->first, level + 1);

            printf("%*sblock:\n", level*3, " ");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_BIN_OP) : {
            printf("%*sBIN_EXPR:\n", level++*3, " ");

            printf("%*soperator: %s.\n", level*3, " ", who->operation);

            printf("%*s1operand:\n", level*3, " ");
            dumpAST(who->first, level + 1);

            printf("%*s2operand:\n", level*3, " ");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_UN_OP) : {
            printf("%*sUN_EXPR:\n", level*3, " ");
            printf("%*soperator: %s.\n", level*3, " ", who->operation);
            printf("%*soperand:\n", level*3, " ");
            dumpAST(who->first, level + 1);
            break;
        }
        case (N_INT) : {
            printf("%*sINT: %d\n", level*3, " ", who->u.i);
            break;
        }
        case (N_FLOAT) : {
            printf("%*sFLOAT: %f\n", level*3, " ", who->u.f);
            break;
        }

        case (N_CHAR) : {
            printf("%*sCHAR: %c\n", level*3, " ", who->u.c);
            break;
        }
        case (N_BOOL) : {
            printf("%*sBOOL:", level*3, " ");
            if (who->u.b) {
                printf("True\n");
            } else {
                printf("False %d\n", who->u.b);
            }
            break;
        }
        case (N_VAR) : {
            if(who->u.sym->type->kind == T_ARRAY && who->first) {
                printf("%*sVAR: %s ", level*3, " ", who->u.sym->string);
                AST *temp = who->first;
                while(temp != NULL) {
                    printf("[%d]", temp->u.i);
                    temp = temp->next;
                }
                // Mejorar esta impresion
                dumpAST(who->first->first, 0);
                printf("\n");
            }
            else if(who->type->kind == T_STRUCT || who->type->kind == T_CONF) {
                printf("%*sVAR: %s", level*3, " ", who->u.sym->string);
                AST *temp = who->first;
                while(temp != NULL) {
                    printf(".%s", temp->u.sym->string);
                    temp = temp->first;
                }
                printf("\n");
            }
            else if(who->type->kind == T_POINTER) {
                int i;
                printf("%*sVAR: ", level*3, " ");
                for(i=0; i < who->first->u.i; i++)
                    printf("*");
                printf("%s\n", who->u.sym->string);
            }
            else {
                printf("%*sVAR: %s ", level*3, " ", who->u.sym->string);
                dumpType(who->u.sym->type);
                printf("\n");
            }
            break;
        }
        case (N_RETURN) : {
            printf("%*sRETURN:\n", level*3, " ");
            printf("%*sexpr:\n", level*3, " ");
            if (who->first) {
                dumpAST(who->first, level + 1);
            }
            break;
        }

        case (N_ASGN) : {
            printf("%*sASSIGN:\n", level++*3, " ");

            printf("%*soperator: %s\n", level*3, " ", who->operation);

            printf("%*sl-value:\n", level*3, " ");
            dumpAST(who->first, level + 1);

            printf("%*sr-value:\n", level*3, " ");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_FOR) : {
            printf("%*sFOR:\n", level++*3, " ");

            printf("%*sstart:\n", level*3, " ");
            dumpAST(who->first, level + 1);

            printf("%*send:\n", level*3, " ");
            dumpAST(who->first->next, level + 1);

            if (who->first->next->next != who->last) {
                printf("%*sstep:\n", level*3, " ");
                dumpAST(who->first->next->next, level + 1);
            }
            printf("%*sblock:\n", level*3, " ");
            dumpAST(who->last, level + 1);
            break;
        }
        case (N_SEQ) : {
            printf("%*sSEQUENCE:\n", level*3, " ");
            dumpSeqChildren(who->first, level+1);
            break;
        }
        case (N_WRITE) : {
            printf("%*sWRITE:\n", level*3, " ");
            if (who->u.sym) {
                printf("%*sstring: %s\n", level*3, " ", who->u.sym->string);
            } else {
                dumpAST(who->first, level);
            }
            break;
        }
        case (N_READ) : {
            printf("%*sREAD:\n", level*3, " ");
            dumpAST(who->first, level);
            if (who->u.sym) {
                printf("%*sstring: %s\n", level*3, " ", who->u.sym->string);
            }
            break;
        }
        case (N_BORN) : {
            printf("%*sBORN:\n", level*3, " ");
            dumpAST(who->first, level);
            break;
        }
        case (N_PUFF) : {
            printf("%*sPUFF:\n", level*3, " ");
            dumpAST(who->first, level);
            break;
        }
        case (N_NEXT) : {
            printf("%*sNEXT:\n", level*3, " ");
            break;
        }
        case (N_BREAK) : {
            printf("%*sBREAK:\n", level*3, " ");
            break;
        }
        case (N_PROGRAM) : {
            printf("%*sPROGRAM:\n", level*3, " ");
            dumpSeqChildren(who->first, level+1);
            break;
        }
        case (N_FUNC) : {
            printf("%*sFUNCTION:\n", level*3, " ");
            printf("%*s", level*3, " ");
            dumpType(who->u.sym->type);
            printf("\n");
            dumpAST(who->first, level);
            break;
        }
        case (N_FCALL) : {
            printf("%*sFUN-CALL:\n", level*3, " ");
            printf("%*s", level*3, " ");
            dumpType(who->u.sym->type);
            printf("\n");
            dumpAST(who->first, level);
            break;
        }
    }
}


AST* newErrorNode(Typetree *error) {
    AST* node = newAST();
    node->tag = N_ERROR;

    node->type = error;

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

AST* newBaseTypeNode(Kind kind) {
    switch(kind) {
        case T_INT: {
            return newIntNode(0);
            break;
        }
        case T_FLOAT: {
            return newFloatNode(0.0);
            break;
        }
        case T_CHAR: {
            return newCharNode('\0');
            break;
        }
        case T_BOOL: {
            return newBoolNode(0);
            break;
        }
    }
}

AST* newFuncNode(Entry *entry, AST* block) {
    AST* node = newAST();
    node->tag = N_FUNC;
    node->u.sym = entry;

    addASTChild(node, block);

    return node;
}

AST* newFunCallNode(Entry *entry) {
    AST* node = newAST();
    node->tag = N_FCALL;
    node->u.sym = entry;

    return node;
}




AST* newProgramNode(AST* defb, AST* pblock) {
    AST* node = newAST();
    node->tag = N_PROGRAM;

    if(defb != NULL)
        addASTChild(node, defb);

    addASTChild(node, pblock);

    return node;
}