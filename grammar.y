%{
#include <stdio.h>
#include "TypeTree/typeTree.h"
#include "SymbolTable/symbolTable.h"

extern int yylineno;
extern char* yytext;

int has_error = 0;

void yyerror (char const *s);
extern int yylex();

Symtable* current = NULL;
Symtable* strings = NULL;

void constant_string(char*);

void declare_var(char*);
void declare_struct(char*);
void declare_proc(char*);
void declare_func(char*);

void check_var(char*);
void check_record(char*);
void check_func(char*);
void check_proc(char*);

%}
%locations
%union {
	int ival;
	float fval;
	char *str;
	char c;
}


%token 	<ival> 	NUMBER
%token 	<fval> 	REAL
%token  <str>   STRING ID SC_ID
%token  <c>     CHARACTER

%token PROGRAM

%token NL

%token PREDEC

%token <str> BOOL CHAR FLOAT INT HOLLOW

%token CONFEDERATION STRUCT

%token IF ELIF ELSE

%token JFALSE JTRUE JNULL

%token FOR WHILE TO STEP BREAK NEXT

%token FUNC PROC REF RETURN

%token BORN PUFF

%token READ WRITE

%token LTOE GTOE EQUAL UNEQUAL AND OR

%token PLUS_ASSIGN MINUS_ASSIGN MULT_ASSIGN DIV_ASSIGN

%token ARROW

%right '=' PLUS_ASSIGN MINUS_ASSIGN MULT_ASSIGN DIV_ASSIGN
%left OR
%left AND
%left EQUAL UNEQUAL
%nonassoc '>' '<' LTOE GTOE
%left '+' '-'
%left '*' '/' '%'
%right UNARY

%start jaxmin

%%

jaxmin
    : opt_nls { current = enterScope(current); } definitions nls PROGRAM block opt_nls { current = exitScope(current); }
    | opt_nls PROGRAM block opt_nls
    ;

opt_nls
    : /* lambda */
    | nls
    ;

nls
    : NL
    | nls NL
    ;

definitions
	: outer_def
    | definitions nls outer_def
	;

outer_def
    : init_dcl
    | subrout_def
    | fwd_dec
    ;

block
    : { current = enterScope(current); } '{' opt_nls ins_list opt_nls '}' { current = exitScope(current); }
    ;

ins_list
    : instruction
    | ins_list nls instruction
    ;

instruction
    : init_dcl
    | block
    | selection
    | iteration
    | assignment
    | jump
    | io_inst
    | malloc
    | proc_call
    ;

malloc
    : BORN '(' ID { check_var($3); } ')'
    | PUFF '(' ID { check_var($3); } ')'
    ;

io_inst
    : READ STRING { constant_string($2); } ',' ID { check_var($5); }
	| READ ID { check_var($2); }
    | WRITE STRING { constant_string($2); }
	| WRITE ID { check_var($2); }
	;

jump
	: NEXT
	| BREAK
	| RETURN
	| RETURN expr
	;

declaration
    : type id_list
    | type ID dimension { declare_var($2); }
    | type point_d ID { declare_var($3); }
    /* pointer to array y vice versa */
    | s_c SC_ID { declare_struct($2); current = enterScope(current); } '{' opt_nls dcl_list opt_nls '}' { current = exitScope(current); }
    | s_c SC_ID ID { declare_var($3); }
    ;

init_dcl
    : type ID '=' expr { declare_var($2); }
    | type ID dimension '=' expr { declare_var($2); }
    | declaration
    ;

id_list
    : ID { declare_var($1); }
    | id_list ',' ID { declare_var($3); }
    ;

dcl_list
    : declaration
    | dcl_list nls declaration
    ;

s_c
    : STRUCT
    | CONFEDERATION
    ;

point_d
    : '*'
    | point_d '*'
    ;

dimension
    : '[' expr ']'
    | dimension '[' expr ']'
    ;

type
    : HOLLOW
    | INT
    | CHAR
    | FLOAT
    | BOOL
    | SC_ID { check_record($1); }
    ;

assignment
    : ID { check_var($1); } '=' expr
    | ID { check_var($1); } PLUS_ASSIGN expr
    | ID { check_var($1); } MINUS_ASSIGN expr
    | ID { check_var($1); } MULT_ASSIGN expr
    | ID { check_var($1); } DIV_ASSIGN expr
    ;

iteration
    : WHILE expr block
    | FOR for_bot_args TO for_args block { current = exitScope(current); }
    | FOR for_bot_args TO for_args STEP NUMBER block { current = exitScope(current); }
    ;

for_bot_args
    : { current = enterScope(current); } for_args

for_args
    : expr
    | init_dcl
    ;

selection
    : IF expr block elif_stm ELSE block
    | IF expr block elif_stm
    | IF expr block ELSE block
    | IF expr block
    ;

elif_stm
    : ELIF expr block
    | elif_stm ELIF expr block
    ;

expr
    : literal
    | ID { check_var($1); }
    | ID dimension { check_var($1); }
    /* | point_d ID { check_var($2); } */
    | ID '.' ID { check_var($1); /* check_field */ }
    | func_call
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '%' expr
    | '-' expr      %prec UNARY
    | '(' expr ')'
    | '~' expr      %prec UNARY
    | expr AND expr
    | expr OR expr
    | expr LTOE expr
    | expr GTOE expr
    | expr EQUAL expr
    | expr UNEQUAL expr
    | expr '<' expr
    | expr '>' expr
    ;

subrout_def
    : FUNC ID { declare_func($2); } '(' f_formals ')' ARROW type block { current = exitScope(current); }
    | PROC ID { declare_proc($2); } '(' p_formals ')' block { current = exitScope(current); }
    ;

fwd_dec
    : PREDEC FUNC ID { declare_func($3); } '(' f_formals ')' ARROW type { current = exitScope(current); }
    | PREDEC PROC ID { declare_proc($3); } '(' p_formals ')' { current = exitScope(current); }
    ;

f_formals
    : /* lambda */ { current = enterScope(current); }
    | { current = enterScope(current); } f_formal_list
    ;

p_formals
    : /* lambda */ { current = enterScope(current); }
    | { current = enterScope(current); } p_formal_list
    ;

f_formal_list
    : f_formal
    | f_formal_list ',' f_formal
    ;

p_formal_list
    : p_formal
    | p_formal_list ',' p_formal
    ;

f_formal
    : type ID { declare_var($2); }
    ;

p_formal
    : type ID { declare_var($2); }
    | REF type ID { declare_var($3); }
    ;

func_call
    : ID { check_func($1); } '(' arguments ')'
    ;

proc_call
    : ID { check_proc($1); } '(' arguments ')'
    ;

arguments
    : /* lambda */
    | args_list
    ;

args_list
    : expr
    | args_list ',' expr
    ;

literal
	: NUMBER
	| REAL
	| CHARACTER
	| JTRUE
	| JFALSE
	| JNULL
	;
%%

void yyerror (char const *s) {
    has_error++;
    fprintf (stderr, "%s %d:%d en %s\n", s, yylineno, yylloc.first_column, yytext);
}

void constant_string(char* str) {
    if(lookupTable(strings, str, 1) == NULL) {
        insertTable(strings, str, yylloc.first_line, yylloc.first_column);
    }
}

void declare_var(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column);
    }
    else {
        fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
            yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        has_error = 1;
    }
}

void declare_struct(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column);
    }
    else {
        fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
            yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        has_error = 1;
    }

}

void declare_proc(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column);
    }
    else {
        if(aux->line) {
            fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarado en linea %d columna %d.\n",
                yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        }
        else {
            fprintf(stderr, "Error:%d:%d: \"%s\" es un procedimiento predefinido por Jaxmin.\n",
                yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        }
        has_error = 1;
    }
}

void declare_func(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column);
    }
    else {
        if(aux->line) {
            fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
                yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        }
        else {
            fprintf(stderr, "Error:%d:%d: \"%s\" es una función predefinida por Jaxmin.\n",
                yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        }
        has_error = 1;
    }
}

void check_var(char *id) {
    if(lookupTable(current, id, 0) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
}

void check_record(char* id) {
    if(lookupTable(current, id, 0) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
}

void check_func(char *id) {
    if(lookupTable(current, id, 0) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
}

void check_proc(char *id) {
    if(lookupTable(current, id, 0) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarado\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
}
