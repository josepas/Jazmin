#include "tac.h"

Quad* generateTAC(TACType tac, Operation op, Addr *arg1, Addr *arg2, Addr *result) {
    Quad *q = (Quad*) memory(sizeof(Quad));
    switch(tac) {
        case BIN_OP:
        case IF_RELOP_JUMP:
        case FUNC_CALL:
        case COPY_FROM_INDEX:
        case COPY_TO_INDEX:
            q->op = op;
            q->arg1 = arg1;
            q->arg2 = arg2;
            q->result = result;
            break;
        case PROC_CALL:
            q->op = op;
            q->arg1 = arg1;
            q->arg2 = arg2;
            break;
        case UNARY_OP:
        case COPY:
        case IF_JUMP:
        case IFN_JUMP:
        case COPY_ADDRESS:
        case COPY_VALUE_POINTED:
        case COPY_TO_POINTED:
            q->op = op;
            q->arg1 = arg1;
            q->result = result;
            break;
        case JUMP:
        case TAC_READ:
        case TAC_REMOVE:
            q->op = op;
            q->result = result;
            break;
        case PARAM:
        case RETURN_VALUE:
        case PRINT:
        case TAC_COMMENT:
        case TAC_LABEL:
            q->op = op;
            q->arg1 = arg1;
            break;
        case TAC_RETURN:
            q->op = op;
            break;
    }

    return q;
}

void addrToString(Addr *a, char *s) {
    switch(a->addt) {
        case CONST_INT:
            sprintf(s, "%d", a->u.i);
            break;
        case CONST_FLOAT:
            sprintf(s, "%f", a->u.f);
            break;
        case CONST_CHAR:
            sprintf(s, "%c", a->u.c);
            break;
        case VAR:
            sprintf(s, "%s", a->u.e->string);
            break;
        case TEMP:
            sprintf(s, "t%d", a->u.t);
            break;
        case LABEL:
            // Para despues
            // sprintf(s, "%d", a->u.i);
            break;
    }
}

// Faltan los nuevos assign
void imprimirTAC(Quad* q) {
    char a1[20], a2[20], r[20];
    switch(q->op) {
        case INT_PLUS:
        case FLOAT_PLUS:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("%s := %s + %s\n", r, a1, a2);
            break;
        case INT_MINUS:
        case FLOAT_MINUS:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("%s := %s - %s\n", r, a1, a2);
            break;
        case INT_MULT:
        case FLOAT_MULT:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("%s := %s * %s\n", r, a1, a2);
            break;
        case INT_DIV:
        case FLOAT_DIV:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("%s := %s / %s\n", r, a1, a2);
            break;
        case INT_MOD:
        case FLOAT_MOD:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("%s := %s %% %s\n", r, a1, a2);
            break;
        case INT_UN_MINUS:
        case FLOAT_UN_MINUS:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("%s := - %s\n", r, a1);
            break;
        case GOTO:
            printf("goto %s\n", ((Entry*)q->result)->string);
            break;
        case IF_GOTO:
            printf("if %s goto %s\n", ((Entry*)q->arg1)->string,
                ((Entry*)q->result)->string);
            break;
        case IFN_GOTO:
            printf("if not %s goto %s\n", ((Entry*)q->arg1)->string,
                ((Entry*)q->result)->string);
            break;
        case OP_EQUAL:
            printf("if %s == %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case OP_UNEQUAL:
            printf("if %s != %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case OP_LT:
            printf("if %s < %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case OP_LTOE:
            printf("if %s <= %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case OP_GT:
            printf("if %s > %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case OP_GTOE:
            printf("if %s >= %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case ASSIGN:
            addrToString(q->result, r);
            addrToString(q->arg1, a1);
            printf("%s := %s\n", r, a1);
            break;
        case OP_REMOVE:
            printf("OP_REMOVE addt:%d  i:%d\n", q->result->addt, q->result->u.i);
            break;
        default:
            printf("MAAAAAL\n");
            break;
    }
}

static unsigned int t_num = 0;

Addr* genTemp(/*struct _symtable* current, struct _typetree* type*/) {
    // char nombre[5];
    // sprintf(nombre, "t%d", t_num);
    // unsigned long h = hash(nombre);
    // Entry *e = (Entry*) memory(sizeof(Entry));
    // e->string = strdup(nombre);
    // e->next = current->table[h];
    // current->table[h] = e;

    Addr *t = (Addr*) malloc(sizeof(Addr));
    t->addt = TEMP;
    t->u.t = t_num++;

    return t;
}

Node* astToTac(AST *ast_node, DLinkedList *list) {
    AST *ast_temp;
    Node *temp, *first, *last;
    Addr *a1, *a2, *r = NULL;
    switch(ast_node->tag) {
        case N_PROGRAM:
            astToTac(ast_node->first, list);
            break;
        case N_SEQ:
            ast_temp = ast_node->first;
            while(ast_temp != NULL) {
                astToTac(ast_temp, list);
                ast_temp = ast_temp->next;
            }
            // if(ast_node->next != NULL)
            //     astToTac(ast_node->next, list);

            break;
        case N_ASGN:
            last = astToTac(ast_node->last, list);
            r = (Addr*)malloc(sizeof(Addr));
            r->addt = VAR;
            r->u.e = ast_node->first->u.sym;
            temp = newDLLNode(
                generateTAC(COPY, ASSIGN,
                    ((Quad*)last->data)->result,
                    NULL,
                    r
                    )
                );
            addDLL(list, temp, 0);
            break;
        case N_UN_OP:
            first = astToTac(ast_node->first, list);

            switch(ast_node->operation[0]) {
                case '-':
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_un_op(INT_UN_MINUS);
                    else
                        temp = def_un_op(FLOAT_UN_MINUS);
                    break;
                case '~':
                    // JUMPING CODE (?)
                    break;
            }
            addDLL(list, temp, 0);
            return temp;
            break;
        case N_BIN_OP:
            first = astToTac(ast_node->first, list);
            last = astToTac(ast_node->last, list);

            switch(ast_node->operation[0]) {
                case '+':
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_PLUS);
                    else
                        temp = def_bin_op(FLOAT_PLUS);
                    break;
                case '-':
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MINUS);
                    else
                        temp = def_bin_op(FLOAT_MINUS);
                    break;
                case '*':
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MULT);
                    else
                        temp = def_bin_op(FLOAT_MULT);
                    break;
                case '/':
                    if(ast_node->operation[1] == '\\') {
                        // JUMPING CODE (?)
                    }
                    else if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_DIV);
                    else
                        temp = def_bin_op(FLOAT_DIV);
                    break;
                case '%':
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MOD);
                    else
                        temp = def_bin_op(FLOAT_MOD);
                    break;
                case '\\':
                    if(ast_node->operation[1] == '/') {
                        // JUMPING CODE (?)
                    }
                    else
                        printf("ERROR!!\n");
                    break;
                case '<':
                    // JUMPING CODE (?)
                    break;
                case '>':
                    // JUMPING CODE (?)
                    break;
                case '=':
                    // JUMPING CODE (?)
                    break;
                case '!':
                    // JUMPING CODE (?)
                    break;
                default:
                    printf("ERROR!!\n");
                    break;
            }


            addDLL(list, temp, 0);
            return temp;

            break;
        case N_INT:
            r = (Addr*)malloc(sizeof(Addr));
            r->addt = CONST_INT;
            r->u.i = ast_node->u.i;

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
        case N_FLOAT:
            r = (Addr*)malloc(sizeof(Addr));
            r->addt = CONST_FLOAT;
            r->u.f = ast_node->u.f;

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
        case N_CHAR:
            r = (Addr*)malloc(sizeof(Addr));
            r->addt = CONST_CHAR;
            r->u.c = ast_node->u.c;

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
        case N_BOOL:
            r = (Addr*)malloc(sizeof(Addr));
            r->addt = CONST_BOOL;
            r->u.b = ast_node->u.b;

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
        case N_VAR:
            r = (Addr*)malloc(sizeof(Addr));
            r->addt = VAR;
            r->u.e = ast_node->u.sym;

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
    }
}
