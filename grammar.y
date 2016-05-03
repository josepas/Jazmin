%{
 #include <stdio.h>
extern int yylineno;
void yyerror (char const *s);
%}
%locations
%union {
	int ival;
	float fval;
	char *idval;
	char *str;
	char c[2];
}


%token 	<ival> 	NUMBER
%token 	<fval> 	REAL
%token 	<idval> ID
%token  <str>   STRING
%token 	<c>		CHARACTER

%token PROGRAM

%token NL

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
    : definitions PROGRAM block 
    | PROGRAM block
    ;

e 
    : /* lambda */
    | NL 
    ;

    

definitions
	: outer_def
    | definitions NL outer_def 
	;

outer_def
    : declaration
    | subrout_def
    | malloc NL
    ;

block
    : e '{' NL ins_list '}'
    ;

ins_list
    : instruction 
    | ins_list instruction
    ;

instruction
    : declaration
    | block
    | selection
    | iteration 
    | assignment NL
    | jump NL
    | io_inst NL
    | malloc NL
    ;

malloc
    : BORN '(' ID ',' type ')' 
    | PUFF '(' ID ')'
    ;

io_inst
	: READ STRING ',' ID
	| WRITE STRING
	;

jump
	: NEXT
	| BREAK
	| RETURN
	| RETURN expr
	;

declaration
    : type ID NL
    | type ID '=' expr NL
    | type ID dimension NL
    | type ID dimension '=' expr NL
    | type point_d ID NL
    | s_c ID '{' dcl_list '}'
    | s_c ID ID NL
    ;

dcl_list
    : declaration
    | dcl_list declaration
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
    : declaration

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
    | expr '+' expr        {printf("holaaa!");}
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
    fprintf (stderr, "%s %d:%d\n", s, yylineno, yylloc.first_column);
}

