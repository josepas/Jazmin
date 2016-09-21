#include "tac.h"

Quad* generateTAC(TACType tac, Operation op, void *arg1, void *arg2, void *result) {
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
        case READ:
            q->op = op;
            q->result = result;
            break;
        case PARAM:
        case RETURN_VALUE:
        case PRINT:
        case COMMENT:
            q->op = op;
            q->arg1 = arg1;
            break;
        case RETURN:
            q->op = op;
            break;
    }

    return q;
}

void imprimirTAC(Quad* q) {
    switch(q->op) {
        case INT_PLUS:
        case FLOAT_PLUS:
            printf("%s := %s + %s\n", ((Entry*)q->result)->string,
                ((Entry*)q->arg1)->string, ((Entry*)q->arg2)->string);
            break;
        case INT_MULT:
        case FLOAT_MULT:
            printf("%s := %s * %s\n", ((Entry*)q->result)->string,
                ((Entry*)q->arg1)->string, ((Entry*)q->arg2)->string);
            break;
        case INT_MINUS:
        case FLOAT_MINUS:
            printf("%s := %s - %s\n", ((Entry*)q->result)->string,
                ((Entry*)q->arg1)->string, ((Entry*)q->arg2)->string);
            break;
        case INT_DIV:
        case FLOAT_DIV:
            printf("%s := %s / %s\n", ((Entry*)q->result)->string,
                ((Entry*)q->arg1)->string, ((Entry*)q->arg2)->string);
            break;
        case INT_MOD:
        case FLOAT_MOD:
            printf("%s := %s %% %s\n", ((Entry*)q->result)->string,
                ((Entry*)q->arg1)->string, ((Entry*)q->arg2)->string);
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
        case EQUAL:
            printf("if %s == %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case UNEQUAL:
            printf("if %s != %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case LT:
            printf("if %s < %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case LTOE:
            printf("if %s <= %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case GT:
            printf("if %s > %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case GTOE:
            printf("if %s >= %s goto %s \n", ((Entry*)q->arg1)->string,
                ((Entry*)q->arg2)->string, ((Entry*)q->result)->string);
            break;
        case ASSIGN:
            printf("%s := %s\n", ((Entry*)q->arg1)->string,
                ((Entry*)q->result)->string);
            break;
        default:
            printf("MAAAAAL\n");
            break;
    }
}

int t_num = 0;

Entry* genTemp(struct _symtable* current, struct _typetree* type) {
    char nombre[5];
    sprintf(nombre, "t%d", t_num);
    unsigned long h = hash(nombre);
    Entry *e = (Entry*) memory(sizeof(Entry));
    e->string = strdup(nombre);
    e->next = current->table[h];

    current->table[h] = e;
    t_num++;

    return e;
}


// Esta aqui para probar que funcione la libreria,
// Quitarlo despues!!
int main() {
    /* Se representara el siguiente programa para probar:
        program
            int a, b, c, d, e
            a = b + c * d - e
    */
    Symtable *tabla = createTable(NULL);

    insertTable(tabla, "a", 1, 1, C_VAR, NULL, 4, 0);
    insertTable(tabla, "b", 1, 2, C_VAR, NULL, 4, 4);
    insertTable(tabla, "c", 1, 3, C_VAR, NULL, 4, 8);
    insertTable(tabla, "d", 1, 4, C_VAR, NULL, 4, 12);
    insertTable(tabla, "e", 1, 5, C_VAR, NULL, 4, 16);

    // c*d
    Quad *q0 = generateTAC(BIN_OP, INT_MULT, lookupTable(tabla, "c", 1),
                    lookupTable(tabla, "d", 1), genTemp(tabla, NULL));
    // b + c * d
    Quad *q1 = generateTAC(BIN_OP, INT_PLUS, lookupTable(tabla, "b", 1),
                    q0->result, genTemp(tabla, NULL));
    // b + c * d - e
    Quad *q2 = generateTAC(BIN_OP, INT_MINUS, q1->result,
                    lookupTable(tabla, "e", 1), lookupTable(tabla, "a", 1));

    imprimirTAC(q0);
    imprimirTAC(q1);
    imprimirTAC(q2);

    return 0;
}