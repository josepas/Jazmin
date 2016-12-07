#include "tac.h"

Labelstack* createLStack() {
    Labelstack *new = (Labelstack*) memory(sizeof(Labelstack));
    new->label = NULL;
    new->prev = NULL;
    return new;
}

void pushLabel(Labelstack *lstack, Addr *label) {
    if(lstack->label == NULL) {
        lstack->label = label;
        return;
    }

    Labelstack *new = (Labelstack*) memory(sizeof(Labelstack));
    new->label = lstack->label;
    new->prev = lstack->prev;
    lstack->label = label;
    lstack->prev = new;
}

Addr* popLabel(Labelstack *lstack) {
    Addr* label = lstack->label;
    if(lstack->prev != NULL) {
        Labelstack *temp = lstack->prev;
        lstack->label = lstack->prev->label;
        lstack->prev = lstack->prev->prev;
        free(temp);
    }
    else {
        lstack->label = NULL;
    }
    return label;
}

Addr* topLabel(Labelstack *lstack) {
    return lstack->label;
}

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
        case CONST_STR:
            address->u.str = (char*)value;
            break;
        case VAR:
            address->u.e = (Entry*)value;
            break;
        case SUBROUTINE:
            address->u.f_name = (char*)value;
            break;
        case LABEL_STR:
            address->u.l_str = *(int*)value;
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
        case TAC_FUNC_CALL:
        case COPY_FROM_INDEX:
        case COPY_TO_INDEX:
            q->op = op;
            q->arg1 = arg1;
            q->arg2 = arg2;
            q->result = result;
            break;
        case TAC_PROC_CALL:
        case TAC_LABEL_STR:
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
        case TAC_ITOF_FTOI:
            q->op = op;
            q->arg1 = arg1;
            q->result = result;
            break;
        case JUMP:
        case TAC_READ:
        case TAC_REMOVE:
        case PARAM:
            q->op = op;
            q->result = result;
            break;
        case TAC_RETURN_VALUE:
        case TAC_WRITE:
        case TAC_LABEL:
        case PRO_EPI_LOGUE:
            q->op = op;
            q->arg1 = arg1;
            break;
        case TAC_RETURN:
        case TAC_EXIT:
        case TAC_COMMENT:
            q->op = op;
            break;
        default:
            printf("ERROR!! NO EXISTE ESTE TAC: %d\n", tac);
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
            sprintf(s, "\'%c\'", a->u.c);
            break;
        case CONST_BOOL:
            if(a->u.b)
                sprintf(s, "True");
            else
                sprintf(s, "False");
            break;
        case CONST_STR:
            sprintf(s, "%s", a->u.str);
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
        case SUBROUTINE:
            sprintf(s, "%s", a->u.f_name);
            break;
        case LABEL_STR:
            sprintf(s, "_str%d", a->u.l_str);
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
            if(q->arg1->addt == SUBROUTINE)
                printf("\n%s:\n", a1);
            else
                printf("%s:\n", a1);
            break;
        case OP_EXIT:
            printf("   exit\n");
            break;
        case OP_FROM_FP:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   %s := *FP(%s)\n", r, a1);
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
        case OP_PARAM:
            addrToString(q->result, r);
            printf("   param %s\n", r);
            break;
        case OP_FUNC_CALL:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            addrToString(q->result, r);
            printf("   %s := call_func %s %s\n", r, a1, a2);
            break;
        case OP_PROC_CALL:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            printf("   call_proc %s %s\n", a1, a2);
            break;
        case OP_RETURN_VALUE:
            addrToString(q->arg1, a1);
            printf("   return %s\n", a1);
            break;
        case EPILOGUE:
            addrToString(q->arg1, a1);
            printf("   epilogue %s\n", a1);
            break;
        case PROLOGUE:
            addrToString(q->arg1, a1);
            printf("   prologue %s\n", a1);
            break;
        case OP_RETURN:
            printf("   return\n");
            break;
        case OP_COMMENT:
            printf("\n### Instruccion\n");
            break;
        case OP_REMOVE:
            printf("OP_REMOVE addt:%d\n", q->result->addt);
            break;
        case OP_WRITE_INT:
            addrToString(q->arg1, a1);
            printf("   _write_int %s\n", a1);
            break;
        case OP_WRITE_FLOAT:
            addrToString(q->arg1, a1);
            printf("   _write_float %s\n", a1);
            break;
        case OP_WRITE_CHAR:
            addrToString(q->arg1, a1);
            printf("   _write_char %s\n", a1);
            break;
        case OP_WRITE_BOOL:
            addrToString(q->arg1, a1);
            printf("   _write_bool %s\n", a1);
            break;
        case OP_WRITE_STR:
            addrToString(q->arg1, a1);
            printf("   _write_str %s\n", a1);
            break;
        case OP_READ_INT:
            addrToString(q->result, a1);
            printf("   _read_int %s\n", a1);
            break;
        case OP_READ_FLOAT:
            addrToString(q->result, a1);
            printf("   _read_float %s\n", a1);
            break;
        case OP_READ_CHAR:
            addrToString(q->result, a1);
            printf("   _read_char %s\n", a1);
            break;
        case OP_READ_BOOL:
            addrToString(q->result, a1);
            printf("   _read_bool %s\n", a1);
            break;
        case OP_STR:
            addrToString(q->arg1, a1);
            addrToString(q->arg2, a2);
            printf("   %s: \"%s\"\n", a1, a2);
            break;
        case CLEANUP:
            addrToString(q->arg1, a1);
            printf("   cleanup %s\n", a1);
            break;
        case OP_ITOF:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   %s := _itof %s\n", r, a1);
            break;
        case OP_FTOI:
            addrToString(q->arg1, a1);
            addrToString(q->result, r);
            printf("   %s := _ftoi %s\n", r, a1);
            break;
        default:
            printf("Operacion inexistente \"%d\"\n", q->op);
            exit(-1);
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
static Operation op;
static int true_value = 1;
static int false_value = 0;

Node* astToTac(AST *ast_node, DLinkedList *list, Addr *true, Addr *false, Addr *next, Context context, Addr *epilogue) {
    static Labelstack *nexts = NULL;
    if(nexts == NULL)
        nexts = createLStack();
    static Labelstack *breaks = NULL;
    if(breaks == NULL)
        breaks = createLStack();

    AST *ast_aux;
    Typetree type_aux;
    Node *temp, *first, *last;
    Addr *a1, *a2, *r, *label_temp, *new_true, *new_false;
    switch(ast_node->tag) {
        case N_PROGRAM:
            // Label de inicio
            r = generateAddr(SUBROUTINE, "program");
            addDLL(list, def_label(r), 0);
            // Label del final

            next = genLabel();

            ast_node = ast_node->first;
            while(ast_node != NULL) {
                astToTac(ast_node, list, next, next, next, context, epilogue);
                ast_node = ast_node->next;
            }

            temp = def_label(next);
            addDLL(list, temp, 0);
            temp = def_exit();
            addDLL(list, temp, 0);
            break;
        case N_SEQ:
            ast_node = ast_node->first;
            label_temp = next;
            while(ast_node != NULL) {
                // addDLL(list, def_comment(), 0);
                // Etiqueta proxima instruccion
                if(ast_node->next)
                    next = genLabel();
                else
                    next = label_temp;

                temp = astToTac(ast_node, list, NULL, NULL, next, context, epilogue);

                // Pegamos etiqueta
                if(ast_node = ast_node->next)
                    addDLL(list, def_label(next), 0);
            }
            return temp;
            break;
        case N_ASGN:
            lrvalue = R_VALUE;
            if(ast_node->last->type->kind == T_BOOL) {
                new_true = genLabel();
                new_false = genLabel();
            }
            last = astToTac(ast_node->last, list, new_true, new_false, next, context, epilogue);

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
            first = astToTac(ast_node->first, list, NULL, NULL, NULL, context, epilogue);

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
                    if(ast_node->first->u.sym->scope == LOCAL)
                        temp = def_asgn(TAC_FP, OP_TO_FP);
                    else if(ast_node->first->u.sym->scope == GLOBAL)
                        temp = def_asgn(TAC_FP, OP_TO_GP);
                    else
                        temp = def_asgn(COPY, ASSIGN);
                    addDLL(list, temp, 0);
                    break;
            }

            return temp;
            break;
        case N_IF:
            ast_node = ast_node->first;

            while(ast_node && ast_node->next) {
                // Creacion de etiquetas
                new_true = genLabel();
                new_false = next;
                if(ast_node->next->next != NULL)
                    new_false = genLabel();

                astToTac(ast_node, list, new_true, new_false, NULL, B, epilogue);

                addDLL(list, def_label(new_true), 0);

                astToTac(ast_node->next, list, NULL, NULL, next, context, epilogue);

                if(ast_node->next->next) {
                    addDLL(list, def_goto(next), 0);
                    addDLL(list, def_label(new_false), 0);
                }

                ast_node = ast_node->next->next;
            }

            // Tengo else ??
            if(ast_node && ast_node->next == NULL) {
                astToTac(ast_node, list, NULL, NULL, next, context, epilogue);
            }

            break;

        case N_WHILE:
            label_temp = genLabel();    //18
            new_true = genLabel();  // 19

            pushLabel(nexts, label_temp);
            pushLabel(breaks, next);

            addDLL(list, def_label(label_temp), 0);

            astToTac(ast_node->first, list, new_true, next, NULL, context, epilogue);

            addDLL(list, def_label(new_true), 0);

            astToTac(ast_node->first->next, list, NULL, NULL, label_temp, context, epilogue);

            addDLL(list, def_goto(label_temp), 0);

            popLabel(nexts);
            popLabel(breaks);

            break;
        case N_FOR:
            op = OP_LT;
            ast_aux = ast_node->first->next->next;
            if(ast_aux->tag == N_INT)
                op = ast_aux->u.i < 0 ? OP_GT : OP_LT;


            lrvalue = R_VALUE;
            first = astToTac(ast_node->first, list, NULL, NULL, NULL, context, epilogue);

            lrvalue = R_VALUE;
            last = astToTac(ast_node->first->next, list, NULL, NULL, NULL, context, epilogue);

            label_temp = genLabel();
            addDLL(list, def_label(label_temp), 0);

            pushLabel(nexts, label_temp);
            pushLabel(breaks, next);

            new_true = genLabel();

            temp = newDLLNode(
                        generateTAC(IF_RELOP_JUMP, op,
                            ((Quad*)first->data)->result,
                            ((Quad*)last->data)->result,
                            new_true
                            )
                        );
            addDLL(list, temp, 0);

            addDLL(list, def_goto(next), 0);

            addDLL(list, def_label(new_true), 0);

            astToTac(ast_node->first->next->next, list, NULL, NULL, NULL, context, epilogue);

            Addr *a2 = (Addr*)malloc(sizeof(Addr));
            a2->addt = CONST_INT;
            a2->u.i = ast_aux->tag == N_INT ? ast_aux->u.i : 1;
            temp = newDLLNode(
                        generateTAC(BIN_OP, INT_PLUS,
                            ((Quad*)first->data)->result,
                            a2,
                            ((Quad*)first->data)->result
                            )
                        );
            addDLL(list, temp, 0);

            addDLL(list, def_goto(label_temp), 0);

            popLabel(nexts);
            popLabel(breaks);

            break;
        case N_FCALL:
            if(strcmp(ast_node->u.sym->string, "itof") == 0 || strcmp(ast_node->u.sym->string, "ftoi") == 0) {
                lrvalue = R_VALUE;
                temp = astToTac(ast_node->first, list, true, false, next, context, epilogue);

                if(strcmp(ast_node->u.sym->string, "itof") == 0)
                    temp = def_itof_ftoi(OP_ITOF);
                else if(strcmp(ast_node->u.sym->string, "ftoi") == 0)
                    temp = def_itof_ftoi(OP_FTOI);

                addDLL(list, temp, 0);
                return temp;
            }

            ast_aux = ast_node->first;
            int size = funcParamsTAC(ast_aux, list, NULL, NULL, NULL, context, epilogue);

            a1 = (Addr*)malloc(sizeof(Addr));
            a1->addt = SUBROUTINE;
            a1->u.f_name = ast_node->u.sym->string;
            a2 = (Addr*)malloc(sizeof(Addr));
            a2->addt = CONST_INT;
            a2->u.i = size;

            temp = newDLLNode(
                        generateTAC(TAC_FUNC_CALL, OP_FUNC_CALL,
                            a1,
                            a2,
                            genTemp()
                            )
                        );
            addDLL(list, temp, 0);

            temp = newDLLNode(
                        generateTAC(PRO_EPI_LOGUE, CLEANUP,
                            a2,
                            NULL,
                            NULL
                            )
                        );
            addDLL(list, temp, 0);

            return temp;

            break;
        case N_PCALL:
            ast_aux = ast_node->first;
            size = funcParamsTAC(ast_aux, list, NULL, NULL, NULL, context, epilogue);

            a1 = (Addr*)malloc(sizeof(Addr));
            a1->addt = SUBROUTINE;
            a1->u.f_name = ast_node->u.sym->string;
            a2 = (Addr*)malloc(sizeof(Addr));
            a2->addt = CONST_INT;
            a2->u.i = size;

            temp = newDLLNode(
                        generateTAC(TAC_PROC_CALL, OP_PROC_CALL,
                            a1,
                            a2,
                            NULL
                            )
                        );
            addDLL(list, temp, 0);

            temp = newDLLNode(
                        generateTAC(PRO_EPI_LOGUE, CLEANUP,
                            a2,
                            NULL,
                            NULL
                            )
                        );
            addDLL(list, temp, 0);

            return temp;

            break;
        case N_FUNC:
            r = generateAddr(SUBROUTINE, ast_node->u.sym->string);
            addDLL(list, def_label(r), 0);

            // Prologo
            r = generateAddr(CONST_INT, &(ast_node->u.sym->size));

            addDLL(list, def_logue(PROLOGUE, r), 0);

            // Label del epilogo
            epilogue = genLabel();

            // Bloque de instrucciones
            astToTac(ast_node->first, list, epilogue, epilogue, epilogue, N, epilogue);

            // Etiqueta del epilogo
            temp = def_label(epilogue);
            addDLL(list, temp, 0);

            // Epilogo
            addDLL(list, def_logue(EPILOGUE, r), 0);

            break;
        case N_RETURN:
            temp = NULL;
            if(ast_node->first) {
                first = astToTac(ast_node->first, list, true, false, next, E, epilogue);
            }

            temp = newDLLNode(
                        generateTAC(TAC_RETURN_VALUE, OP_RETURN_VALUE,
                            ((Quad*)first->data)->result,
                            NULL,
                            NULL
                            )
                        );
            addDLL(list, temp, 0);

            addDLL(list, def_goto(epilogue), 0);

            return temp;
            break;
        case N_UN_OP:
            switch(ast_node->operation[0]) {
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context, epilogue);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_un_op(INT_UN_MINUS);
                    else
                        temp = def_un_op(FLOAT_UN_MINUS);
                    break;
                    addDLL(list, temp, 0);
                    return temp;
                case '~':
                    first = astToTac(ast_node->first, list, false, true, next, B, epilogue);
                    break;
            }
            break;
        case N_BIN_OP:
            switch(ast_node->operation[0]) {
                case '+':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context, epilogue);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context, epilogue);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_PLUS);
                    else
                        temp = def_bin_op(FLOAT_PLUS);
                    break;
                case '-':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context, epilogue);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context, epilogue);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MINUS);
                    else
                        temp = def_bin_op(FLOAT_MINUS);
                    break;
                case '*':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context, epilogue);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context, epilogue);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MULT);
                    else
                        temp = def_bin_op(FLOAT_MULT);
                    break;
                case '/':
                    if(ast_node->operation[1] == '\\') {
                        new_true = genLabel();
                        astToTac(ast_node->first, list, new_true, false, next, B, epilogue);
                        temp = def_label(new_true);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false, next, B, epilogue);
                        return;
                    }
                    else {
                        first = astToTac(ast_node->first, list, NULL, NULL, next, context, epilogue);
                        last = astToTac(ast_node->last, list, NULL, NULL, next, context, epilogue);
                        if(ast_node->first->type->kind == T_INT)
                            temp = def_bin_op(INT_DIV);
                        else
                            temp = def_bin_op(FLOAT_DIV);
                    }
                    break;
                case '%':
                    first = astToTac(ast_node->first, list, NULL, NULL, next, context, epilogue);
                    last = astToTac(ast_node->last, list, NULL, NULL, next, context, epilogue);
                    if(ast_node->first->type->kind == T_INT)
                        temp = def_bin_op(INT_MOD);
                    else
                        temp = def_bin_op(FLOAT_MOD);
                    break;
                case '\\':
                    if(ast_node->operation[1] == '/') {
                        new_false = genLabel();
                        astToTac(ast_node->first, list, true, new_false, next, B, epilogue);
                        temp = def_label(new_false);
                        addDLL(list, temp, 0);
                        astToTac(ast_node->last, list, true, false, next, B, epilogue);
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
                    first = astToTac(ast_node->first, list, true, false, next, E, epilogue);
                    last = astToTac(ast_node->last, list, true, false, next, E, epilogue);
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
                    first = astToTac(ast_node->first, list, true, false, next, E, epilogue);
                    last = astToTac(ast_node->last, list, true, false, next, E, epilogue);
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
                    lrvalue = R_VALUE;
                    first = astToTac(ast_node->first, list, new_true, new_false, next, E, epilogue);
                    addDLL(list, def_label(new_true), 0);
                    last = astToTac(ast_node->last, list, true, false, next, E, epilogue);
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
                    lrvalue = R_VALUE;
                    first = astToTac(ast_node->first, list, new_true, new_false, next, E, epilogue);
                    addDLL(list, def_label(new_true), 0);
                    last = astToTac(ast_node->last, list, true, false, next, E, epilogue);
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
            // printf("aquii\n");
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
            return generateNodeVar(list, ast_node, ast_node->u.sym->scope, epilogue);
            break;
        case N_WRITE:
                if(ast_node->first != NULL) {
                    lrvalue = R_VALUE;
                    temp = astToTac(ast_node->first, list, true, false, next, context, epilogue);
                    type_aux = *(ast_node->first->type);
                }
                else {
                    r = generateAddr(LABEL_STR, &(ast_node->u.sym->offset));
                    temp = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                        );
                    type_aux.kind = T_HOLLOW;
                }
                switch(type_aux.kind) {
                    case T_INT:
                        temp = def_write(OP_WRITE_INT);
                        break;
                    case T_FLOAT:
                        temp = def_write(OP_WRITE_FLOAT);
                        break;
                    case T_BOOL:
                        temp = def_write(OP_WRITE_BOOL);
                        break;
                    case T_CHAR:
                        temp = def_write(OP_WRITE_CHAR);
                        break;
                    default:
                        temp = def_write(OP_WRITE_STR);
                        break;
                }
                addDLL(list, temp, 0);
            break;
        case N_READ:
                lrvalue = L_VALUE;
                temp = astToTac(ast_node->first, list, true, false, next, context, epilogue);

                switch(ast_node->first->type->kind) {
                    case T_INT:
                        temp = def_read(OP_READ_INT);
                        break;
                    case T_FLOAT:
                        temp = def_read(OP_READ_FLOAT);
                        break;
                    case T_BOOL:
                        temp = def_read(OP_READ_BOOL);
                        break;
                    case T_CHAR:
                        temp = def_read(OP_READ_CHAR);
                        break;
                }
                addDLL(list, temp, 0);
            break;
        case N_NEXT:
            addDLL(list, def_goto(topLabel(nexts)), 0);
            break;
        case N_BREAK:
            addDLL(list, def_goto(topLabel(breaks)), 0);
            break;
        default:
            printf("Nodo no existe: %d\n", ast_node->tag);
            exit(-1);
            break;
    }
}

Node* generateNodeVar(DLinkedList *list, AST *ast_node, Scope scope, Addr *epilogue) {
    AST *ast_aux;
    Node *first, *last, *temp;
    Typetree *type_aux;
    Addr *a1, *a2, *r;
    Context context;
    switch(ast_node->type->kind) {
        case T_ARRAY:
            ast_aux = ast_node->first;
            type_aux = ast_node->type->u.a.t;
            int dimensions = 0;
            Node *temp_aux;

            while(ast_aux) {
                temp_aux = temp;
                ++dimensions;

                first = astToTac(ast_aux, list, NULL, NULL, NULL, context, epilogue);

                r = generateAddr(CONST_INT, &(type_aux->size));
                last = newDLLNode(
                        generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, r)
                    );

                temp = def_bin_op(INT_MULT);
                addDLL(list, temp, 0);

                if(dimensions > 1) {
                    // Esto es para poder usar el macro
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
        case T_STRUCT:
        case T_CONF:
            first = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, generateAddr(CONST_INT, &(ast_node->u.sym->offset)))
                );
            last = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, generateAddr(CONST_INT, &(ast_node->first->u.sym->offset)))
                );
            temp = def_bin_op(INT_PLUS);
            addDLL(list, temp, 0);

            ast_node = ast_node->first->first;

            while(ast_node) {
                first = last;
                last = newDLLNode(
                    generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, generateAddr(CONST_INT, &(ast_node->u.sym->offset)))
                );
                temp = def_bin_op(INT_PLUS);
                addDLL(list, temp, 0);

                ast_node = ast_node->first;
            }
            if(scope == GLOBAL) {
                temp = newDLLNode(
                        generateTAC(TAC_GP, OP_FROM_GP, ((Quad*)temp->data)->result, NULL, genTemp())
                    );
            }
            else {
                temp = newDLLNode(
                        generateTAC(TAC_FP, OP_FROM_FP, ((Quad*)temp->data)->result, NULL, genTemp())
                    );
            }
            addDLL(list, temp, 0);
            break;
        default:    // Es variable normal
            a1 = generateAddr(CONST_INT, &(ast_node->u.sym->offset));
            if(scope == GLOBAL) {
                if(lrvalue == R_VALUE) {
                    temp = newDLLNode(
                            generateTAC(TAC_GP, OP_FROM_GP, a1, NULL, genTemp())
                        );
                    addDLL(list, temp, 0);
                }
                else {
                    temp = newDLLNode(
                            generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, a1)
                        );
                }
            }
            else {
                if(lrvalue == R_VALUE) {
                    temp = newDLLNode(
                            generateTAC(TAC_FP, OP_FROM_FP, a1, NULL, genTemp())
                        );
                    addDLL(list, temp, 0);
                }
                else {
                    temp = newDLLNode(
                            generateTAC(TAC_REMOVE, OP_REMOVE, NULL, NULL, a1)
                        );
                }
            }
            break;
    }
    return temp;
}

void cleanTAC(DLinkedList *list) {
    int jumps[l_num];
    int last_label;
    memset(jumps, 0, sizeof(jumps));

    Node *ins;
    for(ins = list->first; ins != NULL; ins=ins->next) {
        Quad *q = (Quad*)ins->data;
        switch(q->op) {
            case GOTO:
            case IF_GOTO:
            case IFN_GOTO:
            case OP_EQUAL:
            case OP_UNEQUAL:
            case OP_LT:
            case OP_LTOE:
            case OP_GT:
            case OP_GTOE:
                jumps[q->result->u.l] = 1;
                break;
            case OP_LABEL:
                last_label = q->arg1->u.l;
                break;
        }
    }

    for(ins = list->first; ins != NULL; ins=ins->next) {
        Quad *q = (Quad*)ins->data;
        if(q->op == OP_LABEL && q->arg1->addt != SUBROUTINE && q->arg1->u.l != 0 && q->arg1->u.l != last_label && !jumps[q->arg1->u.l]) {
            ins->next->prev = ins->prev;
            ins->prev->next = ins->next;
            // falta free de ese quad
        }
    }
}

int funcParamsTAC(AST *ast, DLinkedList *list, Addr *true, Addr *false, Addr *next, Context context, Addr *epilogue) {
    if(ast == NULL)
        return 0;

    Node *temp, *first, *last;
    int size = funcParamsTAC(ast->next, list, true, false, next, context, epilogue);

    // lrvalue = R_VALUE;
    size += ast->type->size;
    first = astToTac(ast, list, NULL, NULL, NULL, context, epilogue);

    temp = newDLLNode(
            generateTAC(COPY, ASSIGN,
                ((Quad*)first->data)->result,
                NULL,
                genTemp()
                )
            );
    addDLL(list, temp, 0);

    temp = newDLLNode(
            generateTAC(PARAM, OP_PARAM,
                NULL,
                NULL,
                ((Quad*)temp->data)->result
                )
            );
    addDLL(list, temp, 0);

    return size;
}