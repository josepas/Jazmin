%{
#include <stdio.h>
#include <limits.h>
#include <bsd/stdlib.h>
#include "grammar.tab.h"
%}

%option yylineno
%option noyywrap


ID			[[:alpha:]][[:alnum:]_]*
NUMBER 		[+\-]?[[:digit:]]+
FLOAT  		[+\-]?[[:digit:]]+\.[[:digit:]]+


%%


[ \t]+   			/* Nada */
\n 					return NL; printf("Linea nuevaaa!\n");
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
{ID}				printf("ulaa\n");

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

.    	printf("Eres estupido??");

%%

int main(int argc, char const *argv[])
{
   ++argv, --argc;  /* skip over program name */
   if ( argc > 0 )
     yyin = fopen( argv[0], "r" );
   else
     yyin = stdin;

 int token;
  while(token = yylex()) {
      printf("%d\n", token);
  }

   return 0;
}
