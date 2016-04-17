

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
	; 

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





type 
	: HOLLOW
	| INT
	| CHAR
	| FLOAT
	| BOOL
	;


	