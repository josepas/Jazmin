#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include "lex.yy.c"
#include "SymbolTable/symbolTable.h"

extern Symtable* current;
extern Symtable* strings;

int main(int argc, char *argv[]) {

	enum Stages {LEXER, PARSER, SYMBOLS} stage = PARSER;

	int c;
	while ((c = getopt(argc, argv, "lps")) != -1) {
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
            printf("Se econtro %d errores.\n", error_num);
            has_error = 1;
        }
    }

    if (stage == PARSER){
        strings = enterScope(strings);
        current = enterScope(current);

        if ( yyparse() )
    		has_error = 1;
    }

    if (stage == SYMBOLS){
        strings = enterScope(strings);
        current = enterScope(current);

        if ( yyparse() ) {
            has_error = 1;
        } else {
            dumpTable(strings);
            printf("\n>>>>>>>>>>>>>>>>>>>>>>>><\n\n");
            dumpTable(current);
        }
    }

    if (has_error) {
        return(EXIT_FAILURE);
    }
    else {
		return(EXIT_SUCCESS);
    }

}