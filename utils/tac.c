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
        case TAC_FP:
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
        case TAC_EXIT:
            q->op = op;
            break;
        default:
            printf("ERROR!! NO EXISTE ESTE TAC\n");
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
            printf("   %s := %s + %s\n", r, a1, a2);
            break;
        case INT_MINUS:
        case FLOAT_MINUS:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   %s := %s - %s\n", r, a1, a2);
            break;
        case INT_MULT:
        case FLOAT_MULT:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   %s := %s * %s\n", r, a1, a2);
            break;
        case INT_DIV:
        case FLOAT_DIV:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   %s := %s / %s\n", r, a1, a2);
            break;
        case INT_MOD:
        case FLOAT_MOD:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   %s := %s %% %s\n", r, a1, a2);
            break;
        case INT_UN_MINUS:
        case FLOAT_UN_MINUS:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   %s := - %s\n", r, a1);
            break;
        case GOTO:
            addrToString(q->result, r);
            printf("   goto %s\n", r);
            break;
        case IF_GOTO:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   if %s goto %s\n", a1, r);
            break;
        case IFN_GOTO:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   if not %s goto %s\n", a1, r);
            break;
        case OP_EQUAL:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   if %s == %s goto %s \n", a1, a2, r);
            break;
        case OP_UNEQUAL:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   if %s != %s goto %s \n", a1, a2, r);
            break;
        case OP_LT:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   if %s < %s goto %s \n", a1, a2, r);
            break;
        case OP_LTOE:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   if %s <= %s goto %s \n", a1, a2, r);
            break;
        case OP_GT:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   if %s > %s goto %s \n", a1, a2, r);
            break;
        case OP_GTOE:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   if %s >= %s goto %s \n", a1, a2, r);
            break;
        case ASSIGN:
            addrToString(q->result, r);
            addrToString(q->arg1, a1);
            printf("   %s := %s\n", r, a1);
            break;
        case OP_LABEL:
            addrToString(q->arg1, a1);
            printf("%s:\n", a1);
            break;
        case OP_EXIT:
            printf("   exit\n");
            break;
        case OP_REMOVE:
            printf("OP_REMOVE addt:%d\n", q->result->addt);
            break;
        case OP_FP:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   %s := FP(%s)\n", r, a1);
            break;
        default:
            printf("Operacion inexistente \"%d\"\n", q->op);
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

Node* astToTac(AST *ast_node, DLinkedList *list, Addr *true, Addr *false, Addr *next) {
    Node *temp, *first, *last;
    Addr *a1, *a2, *r, *label_temp= NULL;
    switch(ast_node->tag) {
        case N_PROGRAM:
            astToTac(ast_node->first, list, NULL, NULL, next);
            break;
        case N_SEQ:
            ast_node = ast_node->first;
            while(ast_node != NULL) {
                astToTac(ast_node, list, NULL, NULL, label_temp);
                ast_node = ast_node->next;
            }
            break;
        case N_ASGN:
            last = astToTac(ast_node->last, list, NULL, NULL, next);
            break;
        case N_UN_OP:
            switch(ast_node->operation[0]) {
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL, next);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_un_op(INT_UN_MINUS);
                    else
                        temp = def_un_op(FLOAT_UN_MINUS);
                    break;
                    addDLL(list, temp, 0);
                    return temp;
                case '~':
                    first = astToTac(ast_node->first, list, false, true, next);
                    break;
            }
            break;
        case N_BIN_OP:
            switch(ast_node->operation[0]) {
                case '+':
                    first = astToTac(ast_node->first, list, NULL, NULL, next);
                    last = astToTac(ast_node->last, list, NULL, NULL, next);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_PLUS);
                    else
                        temp = def_bin_op(FLOAT_PLUS);
                    break;
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL, next);
                    last = astToTac(ast_node->last, list, NULL, NULL, next);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MINUS);
                    else
                        temp = def_bin_op(FLOAT_MINUS);
                    break;
                case '*':
                    first = astToTac(ast_node->first, list, NULL, NULL, next);
                    last = astToTac(ast_node->last, list, NULL, NULL, next);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MULT);
                    else
                        temp = def_bin_op(FLOAT_MULT);
                    break;
                case '/':
                    if(ast_node->operation[1] == '\\') {
                        Addr *new_true = genLabel();
                        astToTac(ast_node->first, list, new_true, false, next);
                        temp = def_label(new_true);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false, next);
                        return;
                    }
                    else {
                        first = astToTac(ast_node->first, list, NULL, NULL, next);
                        last = astToTac(ast_node->last, list, NULL, NULL, next);
                        if(ast_node->first->type->kind == T_INT)
                            temp = def_bin_op(INT_DIV);
                        else
                            temp = def_bin_op(FLOAT_DIV);
                    }
                    break;
                case '%':
                    first = astToTac(ast_node->first, list, NULL, NULL, next);
                    last = astToTac(ast_node->last, list, NULL, NULL, next);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MOD);
                    else
                        temp = def_bin_op(FLOAT_MOD);
                    break;
                case '\\':
                    if(ast_node->operation[1] == '/') {
                        Addr *new_false = genLabel();
                        astToTac(ast_node->first, list, true, new_false, next);
                        temp = def_label(new_false);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false, next);
                        return;
                    }
                    else
                        printf("ERROR!!\n");
                    break;
                case '<':
                    first = astToTac(ast_node->first, list, true, false, next);
                    last = astToTac(ast_node->last, list, true, false, next);
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
                    first = astToTac(ast_node->first, list, true, false, next);
                    last = astToTac(ast_node->last, list, true, false, next);
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
                    first = astToTac(ast_node->first, list, true, false, next);
                    last = astToTac(ast_node->last, list, true, false, next);
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
                    first = astToTac(ast_node->first, list, true, false, next);
                    last = astToTac(ast_node->last, list, true, false, next);
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
            // printf("==> %d\n", r->u.i);
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
            break;
        case N_VAR:
            if(ast_node->u.sym->scope == LOCAL) {
                switch(ast_node->u.sym->class) {
                    case C_VAR:
                        a1 = (Addr*)malloc(sizeof(Addr));
                        a1->addt = CONST_INT;
                        a1->u.i = ast_node->u.sym->offset;
                        temp = newDLLNode(
                                generateTAC(TAC_FP, OP_FP, a1, NULL, genTemp())
                            );
                        addDLL(list, temp, 0);
                }
            }
            return temp;
            break;
    }
}
