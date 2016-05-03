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
				break;
				stage = PARSER;
			case '?':
				break;
			default:
				abort();
		}
	}
	


	if ( optind == argc-1 ) { 
        FILE *yyin = fopen( argv[optind], "r" );
    }





	if (0) 
		return(EXIT_SUCCESS);
	else 
		return(EXIT_FAILURE);
}