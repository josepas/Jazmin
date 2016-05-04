%{
 #include <stdio.h>

extern int yylineno;
extern char* yytext;

int has_error = 0;

void yyerror (char const *s);
extern int yylex();
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
%token  <str>   STRING
%token  <c>     CHARACTER

%token PROGRAM

%token NL

%token ID SC_ID

%token BOOL CHAR FLOAT INT HOLLOW

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
    : opt_nls definitions nls PROGRAM block opt_nls
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
    : declaration
    | subrout_def
    ;

block
    : '{' opt_nls ins_list opt_nls '}'
    ;

ins_list
    : instruction
    | ins_list nls instruction
    ;

instruction
    : declaration
    | block
    | selection
    | iteration
    | assignment
    | jump
    | io_inst
    | malloc
    ;

malloc
    : BORN '(' ID ',' type ')'
    | PUFF '(' ID ')'
    ;

io_inst
    : READ STRING ',' ID
	| READ ID
    | WRITE STRING
	| WRITE ID
	;

jump
	: NEXT
	| BREAK
	| RETURN
	| RETURN expr
	;

declaration
    : type ID
    | type ID '=' expr
    | type ID dimension
    | type ID dimension '=' expr
    | type point_d ID
    | s_c SC_ID '{' opt_nls dcl_list opt_nls '}'
    | s_c SC_ID ID
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
    : '[' NUMBER ']'
    | dimension '[' NUMBER ']'
    ;

type
    : HOLLOW
    | INT
    | CHAR
    | FLOAT
    | BOOL
    ;

assignment
    : ID '=' expr
    | ID PLUS_ASSIGN expr
    | ID MINUS_ASSIGN expr
    | ID MULT_ASSIGN expr
    | ID DIV_ASSIGN expr
    ;

iteration
    : WHILE expr block
    | FOR for_args TO for_args block
    | FOR for_args TO for_args STEP NUMBER block
    ;

for_args
    : expr
    | declaration
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
    | ID
    | subrout_call
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
    : FUNC ID '(' args_func ')' ARROW type block
    | PROC ID '(' args_proc ')' block
    ;

arg_val
    : type ID
    ;

arg_ref
    : REF arg_val

args_func
    : /* lambda */
    | arg_val
    | args_func ',' arg_val
    ;

args_proc
    : /* lambda */
    | arg_val
    | args_proc ',' arg_val
    | arg_ref
    | args_proc ',' arg_ref
    ;

args_list
    : expr
    | args_list ',' expr
    ;

subrout_call
    : ID '(' args_list ')'
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

void yyerror (char const *s)
{
    has_error++;
    fprintf (stderr, "%s %d:%d en %s\n", s, yylineno, yylloc.first_column, yytext);
}

