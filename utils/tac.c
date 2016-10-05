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
        case CONST_BOOL:
            if(a->u.b)
                sprintf(s, "True");
            else
                sprintf(s, "False");
            break;
        case VAR:
            sprintf(s, "%s", a->u.e->string);
            break;
        case TEMP:
            sprintf(s, "t%d", a->u.t);
            break;
        case LABEL:
            sprintf(s, "L%d", a->u.l);
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
            addrToString(q->result, r);
            printf("goto %s\n", r);
            break;
        case IF_GOTO:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("if %s goto %s\n", a1, r);
            break;
        case IFN_GOTO:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("if not %s goto %s\n", a1, r);
            break;
        case OP_EQUAL:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("if %s == %s goto %s \n", a1, a2, r);
            break;
        case OP_UNEQUAL:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("if %s != %s goto %s \n", a1, a2, r);
            break;
        case OP_LT:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("if %s < %s goto %s \n", a1, a2, r);
            break;
        case OP_LTOE:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("if %s <= %s goto %s \n", a1, a2, r);
            break;
        case OP_GT:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("if %s > %s goto %s \n", a1, a2, r);
            break;
        case OP_GTOE:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("if %s >= %s goto %s \n", a1, a2, r);
            break;
        case ASSIGN:
            addrToString(q->result, r);
            addrToString(q->arg1, a1);
            printf("%s := %s\n", r, a1);
            break;
        case OP_LABEL:
            addrToString(q->arg1, a1);
            printf("%s:  ", a1);
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

static unsigned int l_num = 0;

Addr* genLabel() {
    Addr *l = (Addr*) malloc(sizeof(Addr));
    l->addt = LABEL;
    l->u.l = l_num++;

    return l;
}

Node* astToTac(AST *ast_node, DLinkedList *list, Addr *true, Addr *false) {
    AST *ast_temp;
    Node *temp, *first, *last;
    Addr *a1, *a2, *r = NULL;
    switch(ast_node->tag) {
        case N_PROGRAM:
            astToTac(ast_node->first, list, NULL, NULL);
            break;
        case N_SEQ:
            ast_temp = ast_node->first;
            while(ast_temp != NULL) {
                astToTac(ast_temp, list, NULL, NULL);
                ast_temp = ast_temp->next;
            }
            // if(ast_node->next != NULL)
            //     astToTac(ast_node->next, list);

            break;
        case N_ASGN:
            if(ast_node->last->type->kind == T_BOOL) {
                true = genLabel();
                false = genLabel();
                last = astToTac(ast_node->last, list, true, false);
                temp = newDLLNode(
                    generateTAC(TAC_LABEL, OP_LABEL,
                        true,
                        NULL,
                        NULL
                        )
                    );

                addDLL(list, temp, 0);
                a1 = (Addr*)malloc(sizeof(Addr));
                a1->addt = CONST_BOOL;
                a1->u.b = 1;
                r = (Addr*)malloc(sizeof(Addr));
                r->addt = VAR;
                r->u.e = ast_node->first->u.sym;
                temp = newDLLNode(
                    generateTAC(COPY, ASSIGN,
                        a1,
                        NULL,
                        r
                        )
                    );
                addDLL(list, temp, 0);
                // temp = newDLLNode(
                //     generateTAC(JUMP, GOTO,
                //         NULL,
                //         NULL,
                //         true
                //         )
                //     );
                addDLL(list, temp, 0);
                temp= newDLLNode(
                    generateTAC(TAC_LABEL, OP_LABEL,
                        false,
                        NULL,
                        NULL
                        )
                    );
                addDLL(list, temp, 0);
                a1 = (Addr*)malloc(sizeof(Addr));
                a1->addt = CONST_BOOL;
                a1->u.b = 0;
                temp = newDLLNode(
                    generateTAC(COPY, ASSIGN,
                        a1,
                        NULL,
                        r
                        )
                    );
                addDLL(list, temp, 0);
            }
            else {
                last = astToTac(ast_node->last, list, NULL, NULL);
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
            }
            break;
        case N_UN_OP:

            switch(ast_node->operation[0]) {
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_un_op(INT_UN_MINUS);
                    else
                        temp = def_un_op(FLOAT_UN_MINUS);
                    break;
                    addDLL(list, temp, 0);
                    return temp;
                case '~':
                    first = astToTac(ast_node->first, list, false, true);
                    break;
            }
            break;
        case N_BIN_OP:
            switch(ast_node->operation[0]) {
                case '+':
                    first = astToTac(ast_node->first, list, NULL, NULL);
                    last = astToTac(ast_node->last, list, NULL, NULL);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_PLUS);
                    else
                        temp = def_bin_op(FLOAT_PLUS);
                    break;
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL);
                    last = astToTac(ast_node->last, list, NULL, NULL);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MINUS);
                    else
                        temp = def_bin_op(FLOAT_MINUS);
                    break;
                case '*':
                    first = astToTac(ast_node->first, list, NULL, NULL);
                    last = astToTac(ast_node->last, list, NULL, NULL);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MULT);
                    else
                        temp = def_bin_op(FLOAT_MULT);
                    break;
                case '/':
                    if(ast_node->operation[1] == '\\') {
                        Addr *new_true = genLabel();
                        astToTac(ast_node->first, list, new_true, false);
                        temp = def_label(new_true);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false);
                        return;
                    }
                    else {
                        first = astToTac(ast_node->first, list, NULL, NULL);
                        last = astToTac(ast_node->last, list, NULL, NULL);
                        if(ast_node->first->type->kind == T_INT)
                            temp = def_bin_op(INT_DIV);
                        else
                            temp = def_bin_op(FLOAT_DIV);
                    }
                    break;
                case '%':
                    first = astToTac(ast_node->first, list, NULL, NULL);
                    last = astToTac(ast_node->last, list, NULL, NULL);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MOD);
                    else
                        temp = def_bin_op(FLOAT_MOD);
                    break;
                case '\\':
                    if(ast_node->operation[1] == '/') {
                        Addr *new_false = genLabel();
                        astToTac(ast_node->first, list, true, new_false);
                        temp = def_label(new_false);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false);
                        return;
                    }
                    else
                        printf("ERROR!!\n");
                    break;
                case '<':
                    first = astToTac(ast_node->first, list, true, false);
                    last = astToTac(ast_node->last, list, true, false);
                    if(ast_node->operation[1] == '=') {
                        temp = newDLLNode(
                            generateTAC(IF_RELOP_JUMP, OP_LTOE,
                                ((Quad*)first->data)->result,
                                ((Quad*)last->data)->result,
                                true
                                )
                            );
                    }
                    else {
                        temp = newDLLNode(
                            generateTAC(IF_RELOP_JUMP, OP_LT,
                                ((Quad*)first->data)->result,
                                ((Quad*)last->data)->result,
                                true
                                )
                            );
                    }
                    addDLL(list, temp, 0);
                    temp = newDLLNode(
                        generateTAC(JUMP, GOTO,
                            NULL,
                            NULL,
                            false
                            )
                        );
                    addDLL(list, temp, 0);
                    break;
                case '>':
                    first = astToTac(ast_node->first, list, true, false);
                    last = astToTac(ast_node->last, list, true, false);
                    if(ast_node->operation[1] == '=') {
                        temp = newDLLNode(
                            generateTAC(IF_RELOP_JUMP, OP_GTOE,
                                ((Quad*)first->data)->result,
                                ((Quad*)last->data)->result,
                                true
                                )
                            );
                    }
                    else {
                        temp = newDLLNode(
                            generateTAC(IF_RELOP_JUMP, OP_GT,
                                ((Quad*)first->data)->result,
                                ((Quad*)last->data)->result,
                                true
                                )
                            );
                    }
                    addDLL(list, temp, 0);
                    temp = newDLLNode(
                        generateTAC(JUMP, GOTO,
                            NULL,
                            NULL,
                            false
                            )
                        );
                    addDLL(list, temp, 0);
                    break;
                case '=':
                    first = astToTac(ast_node->first, list, true, false);
                    last = astToTac(ast_node->last, list, true, false);
                    temp = newDLLNode(
                        generateTAC(IF_RELOP_JUMP, OP_EQUAL,
                            ((Quad*)first->data)->result,
                            ((Quad*)last->data)->result,
                            true
                            )
                        );
                    addDLL(list, temp, 0);
                    temp = newDLLNode(
                        generateTAC(JUMP, GOTO,
                            NULL,
                            NULL,
                            false
                            )
                        );
                    addDLL(list, temp, 0);
                    break;
                case '!':
                    first = astToTac(ast_node->first, list, true, false);
                    last = astToTac(ast_node->last, list, true, false);
                    temp = newDLLNode(
                        generateTAC(IF_RELOP_JUMP, OP_UNEQUAL,
                            ((Quad*)first->data)->result,
                            ((Quad*)last->data)->result,
                            true
                            )
                        );
                    addDLL(list, temp, 0);
                    temp = newDLLNode(
                        generateTAC(JUMP, GOTO,
                            NULL,
                            NULL,
                            false
                            )
                        );
                    addDLL(list, temp, 0);
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
            // r = (Addr*)malloc(sizeof(Addr));
            // r->addt = CONST_BOOL;
            // r->u.b = ast_node->u.b;

            // temp = newDLLNode(
            //         generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
            //     );
            // return temp;
            if(ast_node->u.b) {
                temp = newDLLNode(
                    generateTAC(JUMP, GOTO,
                        NULL,
                        NULL,
                        true
                        )
                    );
            }
            else {
                temp = newDLLNode(
                    generateTAC(JUMP, GOTO,
                        NULL,
                        NULL,
                        false
                        )
                    );
            }
            addDLL(list, temp, 0);
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
