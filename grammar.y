%{
	int yylex(void)

%}

%token NL  BOOL  BORN  BREAK  CHAR  CONFEDERATION  ELIF  ELSE
%token FALSE  FLOAT  FOR FUNC  HOLLOW  IF  INT  NEXT  NULL  PROC  
%token PUFF  READ  REF  RETURN  STEP  STRUCT TO  TRUE  WHILE  WRITE  
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
	| FALSE
	| FLOAT
	| FOR
	| FUNC
	| HOLLOW
	| IF
	| INT
	| NEXT
	;

%%