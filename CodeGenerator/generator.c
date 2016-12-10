#include "generator.h"
// #include "genWrapper.h"


void generateMIPS(DLinkedList* list) {

	for (Node* aux = list->first; aux != NULL; aux = aux->next) {
		//imprimirTAC( (Quad*)aux->data);
	}

 

}

bool isint(Addr* add) {
	return add->addt == CONST_INT;
}

bool istmp(Addr* add) {
	return add->addt == TEMP;
}

bool isfloat(Addr* add) {
	return add->addt == CONST_FLOAT;
}

bool ischar(Addr* add) {
	return add->addt == CONST_CHAR;
}






void int_sum_MIPS(Addr *arg1, Addr *arg2, Addr *result) {
	
	// t0 = 1 + 1
	if ( isint(arg1) && isint(arg1) ) {



	// t0 = 1 + t1
	} else if ( isint(arg1) && istmp(arg2) ) {



	// t0 = t1 + 1
	} else if ( istmp(arg1) && isint(arg2) ) {


	// t0 = t1 + t2
	} else if ( istmp(arg1) && istmp(arg2) ) {

	
	// caso no contenplado
	} else {
		fprintf(stderr, "ERROR: caso no contemplado en MIPS de la suma\n");
	}
}



void TACToMIPS (Quad* q) {
    switch(q->op) {
        case INT_PLUS:
        	int_sum_MIPS(q->arg1, q->arg2, q->result);

        	break;
        // case FLOAT_PLUS:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s := %s + %s\n", r, a1, a2);
        //     break;
        // case INT_MINUS:
        // case FLOAT_MINUS:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s := %s - %s\n", r, a1, a2);
        //     break;
        // case INT_MULT:
        // case FLOAT_MULT:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s := %s * %s\n", r, a1, a2);
        //     break;
        // case INT_DIV:
        // case FLOAT_DIV:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s := %s / %s\n", r, a1, a2);
        //     break;
        // case INT_MOD:
        // case FLOAT_MOD:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s := %s %% %s\n", r, a1, a2);
        //     break;
        // case INT_UN_MINUS:
        // case FLOAT_UN_MINUS:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   %s := - %s\n", r, a1);
        //     break;
        // case GOTO:
        //     addrToString(q->result, r);
        //     printf("   goto %s\n", r);
        //     break;
        // case IF_GOTO:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   if %s goto %s\n", a1, r);
        //     break;
        // case IFN_GOTO:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   if not %s goto %s\n", a1, r);
        //     break;
        // case OP_EQUAL:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   if %s == %s goto %s \n", a1, a2, r);
        //     break;
        // case OP_UNEQUAL:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   if %s != %s goto %s \n", a1, a2, r);
        //     break;
        // case OP_LT:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   if %s < %s goto %s \n", a1, a2, r);
        //     break;
        // case OP_LTOE:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   if %s <= %s goto %s \n", a1, a2, r);
        //     break;
        // case OP_GT:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   if %s > %s goto %s \n", a1, a2, r);
        //     break;
        // case OP_GTOE:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   if %s >= %s goto %s \n", a1, a2, r);
        //     break;
        // case ASSIGN:
        //     addrToString(q->result, r);
        //     addrToString(q->arg1, a1);
        //     printf("   %s := %s\n", r, a1);
        //     break;
        // case OP_LABEL:
        //     addrToString(q->arg1, a1);
        //     if(q->arg1->addt == SUBROUTINE)
        //         printf("\n%s:\n", a1);
        //     else
        //         printf("%s:\n", a1);
        //     break;
        // case OP_EXIT:
        //     printf("   exit\n");
        //     break;
        // case OP_FROM_FP:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   %s := *FP(%s)\n", r, a1);
        //     break;
        // case OP_TO_FP:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   FP(%s) := %s\n", r, a1);
        //     break;
        // case OP_FROM_GP:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   %s := GP(%s)\n", r, a1);
        //     break;
        // case OP_TO_GP:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   GP(%s) := %s\n", r, a1);
        //     break;
        // case ASSIGN_FROM_ARRAY:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s := %s[%s]\n", r, a1, a2);
        //     break;
        // case ASSIGN_TO_ARRAY:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s[%s] := %s\n", r, a1, a2);
        //     break;
        // case OP_PARAM:
        //     addrToString(q->result, r);
        //     printf("   param %s\n", r);
        //     break;
        // case OP_FUNC_CALL:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     addrToString(q->result, r);
        //     printf("   %s := call_func %s %s\n", r, a1, a2);
        //     break;
        // case OP_PROC_CALL:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     printf("   call_proc %s %s\n", a1, a2);
        //     break;
        // case OP_RETURN_VALUE:
        //     addrToString(q->arg1, a1);
        //     printf("   return %s\n", a1);
        //     break;
        // case EPILOGUE:
        //     addrToString(q->arg1, a1);
        //     printf("   epilogue %s\n", a1);
        //     break;
        // case PROLOGUE:
        //     addrToString(q->arg1, a1);
        //     printf("   prologue %s\n", a1);
        //     break;
        // case OP_RETURN:
        //     printf("   return\n");
        //     break;
        // case OP_COMMENT:
        //     printf("\n### Instruccion\n");
        //     break;
        // case OP_REMOVE:
        //     printf("OP_REMOVE addt:%d\n", q->result->addt);
        //     break;
        // case OP_WRITE_INT:
        //     addrToString(q->arg1, a1);
        //     printf("   _write_int %s\n", a1);
        //     break;
        // case OP_WRITE_FLOAT:
        //     addrToString(q->arg1, a1);
        //     printf("   _write_float %s\n", a1);
        //     break;
        // case OP_WRITE_CHAR:
        //     addrToString(q->arg1, a1);
        //     printf("   _write_char %s\n", a1);
        //     break;
        // case OP_WRITE_BOOL:
        //     addrToString(q->arg1, a1);
        //     printf("   _write_bool %s\n", a1);
        //     break;
        // case OP_WRITE_STR:
        //     addrToString(q->arg1, a1);
        //     printf("   _write_str %s\n", a1);
        //     break;
        // case OP_READ_INT:
        //     addrToString(q->result, a1);
        //     printf("   _read_int %s\n", a1);
        //     break;
        // case OP_READ_FLOAT:
        //     addrToString(q->result, a1);
        //     printf("   _read_float %s\n", a1);
        //     break;
        // case OP_READ_CHAR:
        //     addrToString(q->result, a1);
        //     printf("   _read_char %s\n", a1);
        //     break;
        // case OP_READ_BOOL:
        //     addrToString(q->result, a1);
        //     printf("   _read_bool %s\n", a1);
        //     break;
        // case OP_STR:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->arg2, a2);
        //     printf("   %s: \"%s\"\n", a1, a2);
        //     break;
        // case CLEANUP:
        //     addrToString(q->arg1, a1);
        //     printf("   cleanup %s\n", a1);
        //     break;
        // case OP_ITOF:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   %s := _itof %s\n", r, a1);
        //     break;
        // case OP_FTOI:
        //     addrToString(q->arg1, a1);
        //     addrToString(q->result, r);
        //     printf("   %s := _ftoi %s\n", r, a1);
        //     break;
        default:
            printf("No se genera MIPS para ese TAC\"%d\"\n", q->op);
            exit(-1);
            break;
    }
}

