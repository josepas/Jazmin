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
const char * tokens[] = {"NUMBER", "REAL", "ID", "CHARACTER", "STRING", "NL", "BOOL", "BORN", "BREAK", "CHAR", "CONFEDERATION", "ELIF", "ELSE", "JFALSE", "FLOAT", "FOR", "FUNC", "HOLLOW", "IF", "INT", "NEXT", "JNULL", "PROC", "PUFF", "READ", "REF", "RETURN", "STEP", "STRUCT", "TO", "JTRUE", "WHILE", "WRITE", "LTOE", "GTOE", "EQUAL", "UNEQUAL", "PLUS_ASSIGN", "MINUS_ASSIGN", "MULT_ASSIGN", "DIV_ASSIGN", "AND", "OR", "ARROW" };

%}

%option yylineno
%option noyywrap


ID			[[:alpha:]][[:alnum:]_]*
NUMBER 		[+-]?[[:digit:]]+
FLOAT  		[+-]?[[:digit:]]*\.[[:digit:]]+([eE][+-]?[[:digit:]]+)?
STRING      \"([^"\n]|\\\")*\"
BAD_STRING      \"([^"\n]|\\\")*\n
CHARACTER        '(\\0|\\a|\\t|\\n|[[:print:]])'
BAD_CHAR1    '[^'\n]*\n
BAD_CHAR2    '(\\[^0atn][^\n]*|[^\\\n].+)'




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
{ID} 				return ID;

{NUMBER} 			{
						yylval.ival = (int) strtonum(yytext, INT_MIN, INT_MAX, &error);
                        if (yylval.ival) {
    						return NUMBER;
                        }
                        else {
                            return -2;
                        }
					}

{FLOAT}				{
                        errno = 0;
						yylval.fval = strtof(yytext, NULL);
                        if (errno==0)
                            return REAL;
                        else
                            return -3;
					}

{CHARACTER}         {
                        if (yyleng == 3) {
                            yylval.character[0] = '\0';
                            yylval.character[1] = yytext[1];
                        }
                        else if (yyleng == 4) {
                            yylval.character[0] = '\\';
                            yylval.character[1] = yytext[2];
                        }
                        return CHARACTER;
                    }

{BAD_CHAR1}         {
                        error = "Falta esperando por el caracter: '";
                        return -1;
                    }
{BAD_CHAR2}         {
                        error = "Un char no puede estar compuesto por más de un caracter";
                        return -1;
                    }

{STRING}            {
                        // El valor ya esta asignado en el YY_USER_ACTION
                        return STRING;
                    }

{BAD_STRING}        {
                        error = "Falta esperando por el caracter: \"";
                        return -1;
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
<COMMENT><+#		if (!--comment_level) { BEGIN(INITIAL); }

#([^>\n].*\n|\n)   yycolumn = 1;


.		{
            error = "Caracter inesperado";
            return -1;
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
                    "Valor: %c%c\n", yylval.character[0],yylval.character[1]);
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
            if(token > 127)
                printf("Token: %s\n", tokens[token-258]);
            else
                printf("Token: %c\n", token);
        }
    }
	printf("Fila: %d\n"
		"Columna: %d\n\n", yylloc.first_line, yylloc.first_column);
}

void print_error(int errort) {
    if(errort == -1)
        fprintf(stderr, "Error:%d:%d:%s\n\n",
            yylineno, yylloc.first_column, error);
    else if (errort == -2)
        fprintf(stderr, "Error:%d:%d:El entero %s no es valido (%s)\n\n",
                yylineno, yylloc.first_column, yytext, error);
    else if (errort == -3) {
        if (yylval.fval == 0)
            fprintf(stderr, "Error:%d:%d:El flotante %s no es valido (underflow)\n\n",
                    yylineno, yylloc.first_column, yytext);
        else
            fprintf(stderr, "Error:%d:%d:El flotante %s no es valido (overflow)\n\n",
                    yylineno, yylloc.first_column, yytext);
    }

}

int main(int argc, char const *argv[]) {
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
        yyin = fopen( argv[0], "r" );
    else
        yyin = stdin;

    int token;

    while((token = yylex())) {
        if(token < 0) {
            error_num++;
            print_error(token);
        }
        else {
            print_token(token);
        }
	}

    fprintf(stderr, "Se encontraron %d errores.\n", error_num);
    return 0;

}
