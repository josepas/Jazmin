%{
 #include <stdio.h>
 void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }
%}
%locations
%union {
	int ival;
	float fval;
	char *idval;
	char *str;
	char character;
}


%token 	<ival> 	NUMBER
%token 	<fval> 	REAL
%token 	<idval> ID
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


%%

expr
    : literal
    | expr '+' expr          
    | expr '-' expr         
    | expr '*' expr          
    | expr '/' expr           
    | expr '%' expr           
    | '-' expr
    | '(' expr ')'        
    | '~'' expr                 
    | expr AND expr 
    | expr OR expr    
    | expr LTOE expr    
    | expr GTOE expr    
    | expr EQUAL expr         
    | expr UNEQUAL expr       
    | expr '<' expr         
    | expr '>' expr         
    ;

literal
	: NUMBER
	| REAL
	| CHARACTER
	| ID
	| STRING     /*ESTO NO SE SI VA AQUI*/
	| TRUE
	| FALSE
	| NULL 
	;


%%

