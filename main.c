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
				goto lexer;
				break;
			case 'p':
				stage = PARSER;
				goto parser;
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

	lexer:;
    int token;
    while (token = yylex()) {
    	print_token(token);
    }
    return 0;

    parser:
    printf("estoy en el parser\n");
    yyparse();



	if (0) 
		return(EXIT_SUCCESS);
	else 
		return(EXIT_FAILURE);
}