#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include "lex.yy.c"

int main(int argc, char *argv[]) {

	enum Stages {LEXER, PARSER} stage = PARSER;

	int c;
	while ((c = getopt(argc, argv, "lp")) != -1) {
		switch (c) {
			case 'l':
				stage = LEXER;
				break;
			case 'p':
				stage = PARSER;
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
    extern int error_num;

    if (stage == LEXER) {
        int token;
        while ( ( token = yylex() ) ) {
        	print_token(token);
        }
        if (!error_num) {
            printf("Se econtraron %d errores.\n", error_num);
            has_error = 1;
            return(EXIT_FAILURE);
        }
        return(EXIT_SUCCESS);
    }

    yyparse();


	if (has_error)
        return(EXIT_FAILURE);
    else
		return(EXIT_SUCCESS);

}