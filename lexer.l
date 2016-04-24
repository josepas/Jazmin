%{
#include <stdio.h>
#include <limits.h>
#include <bsd/stdlib.h>
#include "grammar.tab.h"


int yycolumn = 1;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng; \
    yylval.str = strdup(yytext);

int comment_level = 0;
%}

%option yylineno
%option noyywrap


ID			[[:alpha:]][[:alnum:]_]*
NUMBER 		[+-]?[[:digit:]]+
FLOAT  		[+-]?[[:digit:]]*\.[[:digit:]]+

%x COMMENT

%%


[ \t]+   			/* Nada */
\n 					yycolumn = 1; return NL;
"bool"				return BOOL;
"born"				return BORN;
"break"				return BREAK;
"char"				return CHAR;
"confederation"		return CONFEDERATION;
"elif"				return ELIF;
"else"				return ELSE;
"False"				return JFALSE;
"float"				return FLOAT;
"for"				return FOR;		
"func"				return FUNC;
"hollow"			return HOLLOW;
"if"				return IF;
"int"				return INT;
"next"				return NEXT;
"null"				return JNULL;
"proc"				return PROC;
"puff"				return PUFF;
"read"				return READ;
"ref"				return REF;
"return"			return RETURN;
"step"				return STEP;
"struct"			return STRUCT;
"to"				return TO;
"True"				return JTRUE;
"while"				return WHILE;
"write"				return WRITE;
{ID}				return ID;
{NUMBER} 			return NUMBER;
{FLOAT}				return REAL;

"<="		return LTOE;
">="		return GTOE;
"=="		return EQUAL;
"!="		return UNEQUAL;
"+="		return PLUS_ASSIGN;
"-="		return MINUS_ASSIGN;
"*="		return MULT_ASSIGN;
"/="		return DIV_ASSIGN;
"/\\"		return AND;
"\\/"		return OR;
"->"		return ARROW;

"~"		return '~';
">"		return '>';
"["     return '[';
"]"		return ']';
"{"		return '{';
"}"		return '}';
"("		return '(';
")"		return ')';
"."		return '.';
"="		return '=';
"+"		return '+';
"-"		return '-';
"*"		return '*';
"/"		return '/';
"%"		return '%';
"<"		return '<';

"#>"    {comment_level++; printf("%d----", comment_level); BEGIN(COMMENT); }

<COMMENT>[^#<\n]*  	/* commented stuff */
<COMMENT>\n 		yycolumn = 1; printf("dentro del mundo paralelo %d\n", comment_level);
<COMMENT>#+/#>	   /* wild # that dont open comments I dont think this works*/
<COMMENT>#>			comment_level++;
<COMMENT><*#		if (!--comment_level) { BEGIN(INITIAL); }


.		printf("Caracter Inesperado!\n");


%%

int main(int argc, char const *argv[]) {
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
	 	yyin = fopen( argv[0], "r" );
	else
		yyin = stdin;

	int token;
	char *error;
		
	while(token = yylex()) {
		printf("Token: %s\n"
				"Longitud: %d\n" 
				"Fila: %d\n"
				"Columna: %d\n", yytext, yyleng, yylineno, yylloc.first_column);
	}


   return 0;

}
