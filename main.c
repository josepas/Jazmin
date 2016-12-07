#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include "lex.yy.c"
#include "SymbolTable/symbolTable.h"
#include "AST/ast.h"
#include "utils/tac.h"
#include "List/DoublyLinkedList.h"


extern Symtable* current;
extern Symtable* strings;
extern Typetree* HOLLOW_T;

extern AST* tree;

int main(int argc, char *argv[]) {

	enum Stages {LEXER, PARSER, SYMBOLS, TREE, TAC} stage = PARSER;

	int c;
	while ((c = getopt(argc, argv, "lpsti")) != -1) {
		switch (c) {
			case 'l':
				stage = LEXER;
				break;
			case 'p':
				stage = PARSER;
				break;
            case 's':
                stage = SYMBOLS;
                break;
            case 't':
                stage = TREE;
                break;
            case 'i':
                stage = TAC;
                break;
			case '?':
				break;
			default:
				abort();
		}
	}

	if ( optind == argc-1 )
        yyin = fopen( argv[optind], "r" );
    else
    	yyin = stdin;

    extern int has_error;

    if (stage == LEXER) {
    	extern int error_num;
        int token;
        while ( ( token = yylex() ) ) {
        	print_token(token);
        }
        if (error_num) {
            printf("Se encontraron %d errores.\n", error_num);
            has_error = 1;
        }
    }

    /* Scope 0 */
    strings = enterScope(strings);
    current = enterScope(current);

    insertTable(current, "hollow", 0, 0, C_TYPE, createType(T_HOLLOW), 0, 0);
    insertTable(current, "int", 0, 0, C_TYPE, createType(T_INT), 4, 0);
    insertTable(current, "char", 0, 0, C_TYPE, createType(T_CHAR), 4, 0);
    insertTable(current, "float", 0, 0, C_TYPE, createType(T_FLOAT), 4, 0);
    insertTable(current, "bool", 0, 0, C_TYPE, createType(T_BOOL), 4, 0);
    HOLLOW_T = lookupTable(current, "hollow", 0)->type;
    // insertTable(current, "ftoi", 0, 0);
    ArgList* args = newArgList();
    add(args, lookupTable(current, "int", 0)->type);
    insertTable(current, "itof", 0, 0, C_SUB, createFunc(args, lookupTable(current, "float", 0)->type), 0, 0);
    args = newArgList();
    add(args, lookupTable(current, "float", 0)->type);
    insertTable(current, "ftoi", 0, 0, C_SUB, createFunc(args, lookupTable(current, "int", 0)->type), 0, 0);



    /* Fin scope 0 */

    if (stage == PARSER){
        if ( yyparse() )
            has_error = 1;
    }

    if (stage == SYMBOLS){
        if ( yyparse() ) {
            has_error = 1;
        }

        if(has_error == 0) {
            printf("Strings ");
            dumpTable(strings);
            printf("\n>>>>>>>>>>>>>>>>>>>>>>>><\n\n");
            arreglarTabla(current);
            dumpTable(current);
        }
    }

    if (stage == TREE){
        if ( yyparse() ) {
            has_error = 1;
        }
        if (has_error == 0) {
            arreglarTabla(current);
            dumpAST(tree, 0);
        }
    }

    if (stage == TAC){
        if ( yyparse() ) {
            has_error = 1;
        }
        if (has_error == 0) {
            arreglarTabla(current);
            DLinkedList *list = newDoublyLinkList();

            Entry *aux_entry;
            int i;

            // Strings
            int s_num = 0;
            for(i=0; i<HASH_SIZE; i++) {
                aux_entry = strings->table[i];
                while(aux_entry!=NULL) {
                    aux_entry->offset = s_num++;
                    addDLL(list,
                        newDLLNode(
                            generateTAC(
                                TAC_LABEL_STR,
                                OP_STR,
                                generateAddr(LABEL_STR, &(aux_entry->offset)),
                                generateAddr(CONST_STR, aux_entry->string),
                                NULL
                                )
                            ),
                        0
                    );

                    aux_entry = aux_entry->next;
                }
            }

            // Subrutinas
            for(i=0; i<HASH_SIZE; i++) {
                aux_entry = current->fchild->table[i];
                while(aux_entry!=NULL) {
                    if(aux_entry->class != C_SUB) {
                        aux_entry = aux_entry->next;
                        continue;
                    }

                    astToTac(aux_entry->ast, list, NULL, NULL, NULL, N, NULL);

                    aux_entry = aux_entry->next;
                }
            }

            astToTac(tree, list, NULL, NULL, NULL, N, NULL);

            cleanTAC(list);

            Node* aux;
            for (aux = list->first; aux != NULL; aux = aux->next) {
                // printf("==> %d\n", ((Quad*)aux->data)->op);
                imprimirTAC((Quad*)aux->data);
            }
        }
    }


    if (has_error) {
        return(EXIT_FAILURE);
    }
    else {
		return(EXIT_SUCCESS);
    }

}