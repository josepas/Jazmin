%{
 #include <stdio.h>
 void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }
%}

%token NL  BOOL  BORN  BREAK  CHAR  CONFEDERATION  ELIF  ELSE
%token JFALSE  FLOAT  FOR FUNC  HOLLOW  IF  INT  NEXT  JNULL  PROC  
%token PUFF  READ  REF  RETURN  STEP  STRUCT TO  JTRUE  WHILE  WRITE  
%token LTOE  GTOE  EQUAL  UNEQUAL PLUS_ASSIGN  MINUS_ASSIGN  MULT_ASSIGN  
%token DIV_ASSIGN
%token AND  OR  
%token ARROW  

%%

jaxmin 
	: NL                  
	| BOOL
	| BORN
	| BREAK
	| CHAR
	| CONFEDERATION
	| ELIF
	| ELSE
	| JFALSE
	| FLOAT
	| FOR
	| FUNC
	| HOLLOW
	| IF
	| INT
	| NEXT 
	| JNULL
	| PROC PUFF
	| READ
	| REF
	| RETURN
	| STEP
	| STRUCT TO
	| JTRUE
	| WHILE
	| WRITE
	| LTOE
	| GTOE
	| EQUAL
	| UNEQUAL PLUS_ASSIGN
	| MINUS_ASSIGN
	| MULT_ASSIGN
	| DIV_ASSIGN
	| AND
	| OR
	| ARROW  
	;

%%

