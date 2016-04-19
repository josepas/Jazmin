%{
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);
%}



born puff 
func return proc 
if elif else 
for to step while 
int char bool float
struct
[ ]
{ }
( )
=
+ -
* / %
-
< <= >= > == !=
/\ \/ ~
# #> <#

statement
	: selection
	| iteration
	| jump
	| otras xD 
	| sequence
	;

jump
	: return '\n'
	| break '\n'
	; next '\n'

sequence 
	: '{' '}' 
	| '{' statement_list '}'
	| declaration
	; 

assignment_operator
	: 



statement_list
	: statement
	| statement_list statement
	;

iteration
	: while expr statement 
	;


selection
	: IF expr statement elif_stm ELSE statement
	| IF expr statement elif_stm  
	| IF expr statement ELSE statement
	| IF expr statement  
	;

elif_stm
	: ELIF expr statement 
	| elif_stm ELIF expr statement
	;

expression
    : literal
    | expression '+' expression          
    | expression '-' expression         
    | expression '*' expression          
    | expression '/' expression           
    | expression '%' expression           
    | '-' expression %prec      esto lleva otras cosas
    | '(' expression ')'        
    | '~'' expression                 
    | expression '/\' expression  creo que debo escapar algo alli    
    | expression '\/' expression    
    | expression '<=' expression    
    | expression '>=' expression    
    | expression '==' expression         
    | expression '!=' expression       
    | expression '<' expression         
    | expression '>' expression         
    ;

type 
	: HOLLOW
	| INT
	| CHAR
	| FLOAT
	| BOOL
	;

literal
	: NUMBER
	| CHARACTER
	| STRING     /*ESTO NO SE SI VA AQUI*/
	| TRUE
	| FALSE
	| NULL 
	;
