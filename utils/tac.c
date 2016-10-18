#include "tac.h"

Addr* generateAddr(AddrType addt, void *value) {
    Addr *address = (Addr*)malloc(sizeof(Addr));
    address->addt = addt;
    switch(addt) {
        case CONST_INT:
            address->u.i = *(int*)value;
            break;
        case CONST_FLOAT:
            address->u.f = *(float*)value;
            break;
        case CONST_CHAR:
            address->u.c = *(char*)value;
            break;
        case CONST_BOOL:
            address->u.b = *(int*)value;
            break;
        case VAR:
            address->u.e = (Entry*)value;
            break;
        default:
            printf("Error en \"generateAddr\"\n");
            exit(-1);
            break;
    }
    return address;
}

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
        case TAC_GP:
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
        case TAC_LABEL:
            q->op = op;
            q->arg1 = arg1;
            break;
        case TAC_RETURN:
        case TAC_EXIT:
        case TAC_COMMENT:
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
        default:
            printf("Error en \"addrToString\"\n");
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
        case OP_FROM_FP:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   %s := FP(%s)\n", r, a1);
            break;
        case OP_TO_FP:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   FP(%s) := %s\n", r, a1);
            break;
        case OP_FROM_GP:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   %s := GP(%s)\n", r, a1);
            break;
        case OP_TO_GP:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   GP(%s) := %s\n", r, a1);
            break;
        case ASSIGN_FROM_ARRAY:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   %s := %s[%s]\n", r, a1, a2);
            break;
        case ASSIGN_TO_ARRAY:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   %s[%s] := %s\n", r, a1, a2);
            break;
        case OP_COMMENT:
            printf("\n### Instruccion\n");
            break;
        case OP_REMOVE:
            printf("OP_REMOVE addt:%d\n", q->result->addt);
            break;
        default:
            printf("Operacion inexistente \"%d\"\n", q->op);
            break;
    }
}

static unsigned int t_num = 0;

Addr* genTemp(/*struct _symtable* current, struct _typetree* type*/) {

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

static LRValues lrvalue = NONE;
static int true_value = 1;
static int false_value = 0;

Node* astToTac(AST *ast_node, DLinkedList *list, Addr *true, Addr *false, Addr *next, Context context) {
    AST *ast_aux;
    Typetree *type_aux;
    Node *temp, *first, *last;
    Addr *a1, *a2, *r, *label_temp, *new_true, *new_false;
    switch(ast_node->tag) {
        case N_PROGRAM:
            // Label de inicio
            temp = def_label(genLabel());
            addDLL(list, temp, 0);

            // Label del final
            next = genLabel();

            ast_node = ast_node->first;
            while(ast_node != NULL) {
                astToTac(ast_node, list, next, next, next, context);
                ast_node = ast_node->next;
            }

            temp = def_label(next);
            addDLL(list, temp, 0);
            temp = def_exit();
            addDLL(list, temp, 0);
            break;
        case N_SEQ:
            ast_node = ast_node->first;
            while(ast_node != NULL) {
                addDLL(list, def_comment(), 0);
                astToTac(ast_node, list, next, next, next, context);
                ast_node = ast_node->next;
            }
            break;
        case N_ASGN:
            lrvalue = R_VALUE;
            new_true = genLabel();
            new_false = genLabel();
            last = astToTac(ast_node->last, list, new_true, new_false, next, context);

            if(ast_node->last->type->kind == T_BOOL) {
                addDLL(list, def_label(new_true), 0);
                a1 = generateAddr(CONST_BOOL, &(true_value));
                r = genTemp();
                temp = newDLLNode(
                        generateTAC(COPY, ASSIGN, a1, NULL, r)
                    );
                addDLL(list, temp, 0);

                next = genLabel();
                addDLL(list, def_goto(next), 0);

                addDLL(list, def_label(new_false), 0);
                a1 = generateAddr(CONST_BOOL, &(false_value));
                temp = newDLLNode(
                        generateTAC(COPY, ASSIGN, a1, NULL, r)
                    );
                addDLL(list, temp, 0);


                addDLL(list, def_label(next), 0);
                last = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );

            }

            lrvalue = L_VALUE;
            first = astToTac(ast_node->first, list, NULL, NULL, NULL, context);

            lrvalue = NONE;

            switch(ast_node->first->type->kind) {
                case T_ARRAY:
                    if(ast_node->first->first != NULL) {
                        r = generateAddr(VAR, ast_node->first->u.sym);
                        temp = def_asgn_array(r);
                        addDLL(list, temp, 0);
                    }
                    else {
                        /* Inicializacion completa */
                    }
                    break;
                default:    // Variable normal
                    if(((Quad*)first->data)->op == OP_FROM_FP)
                        temp = def_asgn(TAC_FP, OP_TO_FP);
                    else if(((Quad*)first->data)->op == OP_FROM_GP)
                        temp = def_asgn(TAC_FP, OP_TO_GP);
                    else
                        temp = def_asgn(COPY, ASSIGN);
                    addDLL(list, temp, 0);
                    break;
            }

            return temp;
            break;
        case N_UN_OP:
            switch(ast_node->operation[0]) {
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_un_op(INT_UN_MINUS);
                    else
                        temp = def_un_op(FLOAT_UN_MINUS);
                    break;
                    addDLL(list, temp, 0);
                    return temp;
                case '~':
                    first = astToTac(ast_node->first, list, false, true, next, B);
                    break;
            }
            break;
        case N_BIN_OP:
            switch(ast_node->operation[0]) {
                case '+':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_PLUS);
                    else
                        temp = def_bin_op(FLOAT_PLUS);
                    break;
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MINUS);
                    else
                        temp = def_bin_op(FLOAT_MINUS);
                    break;
                case '*':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MULT);
                    else
                        temp = def_bin_op(FLOAT_MULT);
                    break;
                case '/':
                    if(ast_node->operation[1] == '\\') {
                        new_true = genLabel();
                        astToTac(ast_node->first, list, new_true, false, next, B);
                        temp = def_label(new_true);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false, next, B);
                        return;
                    }
                    else {
                        first = astToTac(ast_node->first, list, NULL, NULL, next, context);
                        last = astToTac(ast_node->last, list, NULL, NULL, next, context);
                        if(ast_node->first->type->kind == T_INT)
                            temp = def_bin_op(INT_DIV);
                        else
                            temp = def_bin_op(FLOAT_DIV);
                    }
                    break;
                case '%':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MOD);
                    else
                        temp = def_bin_op(FLOAT_MOD);
                    break;
                case '\\':
                    if(ast_node->operation[1] == '/') {
                        new_false = genLabel();
                        printf("--->L%d\n", new_false->u.l);
                        astToTac(ast_node->first, list, true, new_false, next, B);
                        temp = def_label(new_false);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false, next, B);
                        return;
                    }
                    else
                        printf("ERROR!!\n");
                    break;
                case '<':
                    if(context == E) {
                        true = genLabel();
                        false = genLabel();
                    }
                    first = astToTac(ast_node->first, list, true, false, next, E);
                    last = astToTac(ast_node->last, list, true, false, next, E);
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

                    if(context == E) {
                        addDLL(list, def_label(true), 0);
                        a1 = generateAddr(CONST_BOOL, &(true_value));
                        r = genTemp();
                        temp = newDLLNode(
                                generateTAC(COPY, ASSIGN, a1, NULL, r)
                            );
                        addDLL(list, temp, 0);

                        next = genLabel();
                        addDLL(list, def_goto(next), 0);

                        addDLL(list, def_label(false), 0);
                        a1 = generateAddr(CONST_BOOL, &(false_value));
                        temp = newDLLNode(
                                generateTAC(COPY, ASSIGN, a1, NULL, r)
                            );
                        addDLL(list, temp, 0);

                        addDLL(list, def_label(next), 0);
                    }

                    return temp;

                    break;
                case '>':
                    if(context == E) {
                        true = genLabel();
                        false = genLabel();
                    }
                    first = astToTac(ast_node->first, list, true, false, next, E);
                    last = astToTac(ast_node->last, list, true, false, next, E);
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

                    if(context == E) {
                        addDLL(list, def_label(true), 0);
                        a1 = generateAddr(CONST_BOOL, &(true_value));
                        r = genTemp();
                        temp = newDLLNode(
                                generateTAC(COPY, ASSIGN, a1, NULL, r)
                            );
                        addDLL(list, temp, 0);

                        next = genLabel();
                        addDLL(list, def_goto(next), 0);

                        addDLL(list, def_label(false), 0);
                        a1 = generateAddr(CONST_BOOL, &(false_value));
                        temp = newDLLNode(
                                generateTAC(COPY, ASSIGN, a1, NULL, r)
                            );
                        addDLL(list, temp, 0);

                        addDLL(list, def_label(next), 0);
                    }

                    return temp;

                    break;
                case '=':
                    new_true = new_false = genLabel();
                    first = astToTac(ast_node->first, list, new_true, new_false, next, E);
                    addDLL(list, def_label(new_true), 0);
                    last = astToTac(ast_node->last, list, true, false, next, E);
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
                    new_true = new_false = genLabel();
                    first = astToTac(ast_node->first, list, new_true, new_false, next, E);
                    addDLL(list, def_label(new_true), 0);
                    last = astToTac(ast_node->last, list, true, false, next, E);
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
            r = generateAddr(CONST_INT, &(ast_node->u.i));

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            // printf("==> %d\n", r->u.i);
            return temp;
            break;
        case N_FLOAT:
            r = generateAddr(CONST_FLOAT, &(ast_node->u.f));

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
        case N_CHAR:
            r = generateAddr(CONST_CHAR, &(ast_node->u.c));

            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
        case N_BOOL:
            if(ast_node->u.b && context != E) {
                temp = def_goto(true);
                addDLL(list, temp, 0);
            }
            else if(context != E) {
                temp = def_goto(false);
                addDLL(list, temp, 0);
            }
            r = generateAddr(CONST_BOOL, &(ast_node->u.b));
            temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                );
            return temp;
            break;
        case N_VAR:
            if(ast_node->u.sym->scope == LOCAL) {
                switch(ast_node->type->kind) {
                    default:    // Es variable normal
                        a1 = generateAddr(CONST_INT, &(ast_node->u.sym->offset));
                        temp = newDLLNode(
                                generateTAC(TAC_FP, OP_FROM_FP, a1, NULL, genTemp())
                            );
                        addDLL(list, temp, 0);
                        break;
                }
            }
            else {
                switch(ast_node->type->kind) {
                    case T_ARRAY:
                        ast_aux = ast_node->first;
                        type_aux = ast_node->type->u.a.t;
                        int dimensions = 0;
                        Node *temp_aux;

                        while(ast_aux) {
                            temp_aux = temp;
                            ++dimensions;

                            first = astToTac(ast_aux, list, NULL, NULL, NULL, context);

                            r = generateAddr(CONST_INT, &(type_aux->size));
                            last = newDLLNode(
                                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                                );

                            temp = def_bin_op(INT_MULT);
                            addDLL(list, temp, 0);

                            if(dimensions > 1) {
                                first = temp_aux;
                                last = temp;
                                temp = def_bin_op(INT_PLUS);
                                addDLL(list, temp, 0);
                            }

                            ast_aux = ast_aux->next;
                            if(type_aux->kind == T_ARRAY)
                                type_aux = type_aux->u.a.t;
                        }

                        if(lrvalue == R_VALUE) {
                            a1 = generateAddr(VAR, ast_node->u.sym);
                            temp = newDLLNode(
                                    generateTAC(COPY_FROM_INDEX, ASSIGN_FROM_ARRAY, a1, ((Quad*)temp->data)->result, genTemp())
                                );
                            addDLL(list, temp, 0);
                        }
                        else if(lrvalue == L_VALUE) {

                        }

                        break;
                    // case T_STRUCT:
                    // case T_CONF:
                        //break;
                    default:    // Es variable normal
                        // r = generateAddr(VAR, ast_node->u.sym);
                        // temp = newDLLNode(
                        //         generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                        //     );
                        a1 = generateAddr(CONST_INT, &(ast_node->u.sym->offset));
                        temp = newDLLNode(
                                generateTAC(TAC_GP, OP_FROM_GP, a1, NULL, genTemp())
                            );
                        addDLL(list, temp, 0);

                        break;
                }
            }
            return temp;
            break;
    }
}
