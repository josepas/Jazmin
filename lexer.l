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
int error_num = 0;

const char *error;

%}

%option yylineno
%option noyywrap
%option nounput
%option noinput


ID          [[:lower:]][[:alnum:]_]*
BAD_ID      [[:digit:]]+[[:alpha:]_][[:alnum:]_]*
STRUCT_ID   [[:upper:]][[:alnum:]_]*
NUMBER 		[[:digit:]]+
FLOAT  		[[:digit:]]*\.[[:digit:]]+([eE][+-]?[[:digit:]]+)?
STRING      \"([^"\n]|\\\")*\"
BAD_STRING      \"([^"\n]|\\\")*\n
CHARACTER        '(\\0|\\a|\\t|\\n|[[:print:]])'
BAD_CHAR1    '[^'\n]*\n
BAD_CHAR2    '(\\[^0atn][^\n]*|[^\\\n].+)'




%x COMMENT

%%
[ \t]+   			/* Nada */
\n  				{
                        yycolumn = 1;
                        yylloc.first_line--;
                        return NL;
                    }

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
"program"           return PROGRAM;
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
"predec"            return PREDEC;

{ID} 				return ID;

{BAD_ID}            {
                        error_num++;
                        fprintf(stderr, "Error:%d:%d:\"%s\" no es un identificador valido\n\n",
                                yylineno, yylloc.first_column, yytext);
                    }

{STRUCT_ID}         return SC_ID;

{NUMBER} 			{
                        errno = 0;
						yylval.ival = (int) strtonum(yytext, INT_MIN, INT_MAX, &error);
                        if (errno == 0) {
    						return NUMBER;
                        }
                        else {
                            error_num++;
                            fprintf(stderr, "Error:%d:%d:El entero %s no es valido (%s)\n\n",
                                    yylineno, yylloc.first_column, yytext, error);
                        }
					}

{FLOAT}				{
                        errno = 0;
						yylval.fval = strtof(yytext, NULL);
                        if (errno==0)
                            return REAL;
                        else {
                            error_num++;
                            if (yylval.fval == 0) {
                                fprintf(stderr, "Error:%d:%d:El flotante %s no es valido (underflow)\n\n",
                                        yylineno, yylloc.first_column, yytext);
                            }
                            else {
                                fprintf(stderr, "Error:%d:%d:El flotante %s no es valido (overflow)\n\n",
                                        yylineno, yylloc.first_column, yytext);
                            }
                        }
					}

{CHARACTER}         {
                        if (yytext[1]=='\\') {
                            switch (yytext[2]) {
                                case ('0'): {
                                    yylval.c = '\0';
                                    break;
                                }
                                case ('a'): {
                                    yylval.c = '\a';
                                    break;
                                }
                                case ('t'): {
                                    yylval.c = '\t';
                                    break;
                                }
                                case ('n'): {
                                    yylval.c = '\n';
                                    break;
                                }
                            }
                        }
                        else {
                            yylval.c = yytext[1];
                        }
                        return CHARACTER;
                    }

{BAD_CHAR1}         {
                        error_num++;
                        fprintf(stderr, "Error:%d:%d:Esperando por el caracter: '\n\n",
                                yylineno, yylloc.first_column);
                    }
{BAD_CHAR2}         {
                        error_num++;
                        fprintf(stderr, "Error:%d:%d:Un char no puede estar compuesto por más de un caracter\n\n",
                                yylineno, yylloc.first_column);
                    }

{STRING}            {
                        // El valor ya esta asignado en el YY_USER_ACTION
                        yylval.str = strdup(++yytext);
                        yylval.str[yyleng-2] = '\0';
                        return STRING;
                    }

{BAD_STRING}        {
                        error_num++;
                        fprintf(stderr, "Error:%d:%d:Esperando por el caracter: \"\n\n",
                                yylineno, yylloc.first_column);
                    }

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
","     return ',';
";"     return ';';
"="		return '=';
"+"		return '+';
"-"		return '-';
"*"		return '*';
"/"		return '/';
"%"		return '%';
"<"		return '<';

"#>"    {comment_level++; BEGIN(COMMENT); }

<COMMENT>[^#<\n]*  	/* commented stuff */
<COMMENT>\n 		yycolumn = 1;
<COMMENT>#+>	   	comment_level++;
<COMMENT><+#		if (!--comment_level) { BEGIN(INITIAL);}

#([^>\n].*\n+|\n+)  {
                        yycolumn = 1;
                        yylloc.first_line--;
                        return NL;
                    }


.		{
            error_num++;
            fprintf(stderr, "Error:%d:%d:Caracter inesperado (%s)\n\n",
                    yylineno, yylloc.first_column, yytext);
        }


%%

void print_token(int token) {
	switch (token) {
		case (ID) : {
			printf("Token: ID\n"
					"Lexema: %s\n", yytext);
			break;
		}
		case (NUMBER) : {
			printf("Token: NUMBER\n"
					"Valor: %d\n", yylval.ival);
			break;
		}
        case (REAL) : {
            printf("Token: REAL\n"
                    "Valor: %f\n", yylval.fval);
            break;
        }
        case (CHARACTER) : {
            printf("Token: CHARACTER\n"
                    "Valor: %c\n", yylval.c);
            break;
        }
        case (STRING) : {
            printf("Token: STRING\n"
                    "Valor: %s\n", yylval.str);
            break;
        }
        case (NL) : {
            printf("Token: NL\n");
            break;
        }
		default : {
            printf("Token: %s\n", yytext);
        }
    }
	printf("Fila: %d\n"
		"Columna: %d\n\n", yylloc.first_line, yylloc.first_column);
}
