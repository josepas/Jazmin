

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








conditional
	: IF expr '{' statement '}' elif_stm ELSE '{' statement '}' 
	| IF expr '{' statement '}' elif_stm  
	| IF expr '{' statement '}' ELSE '{' statement '}'
	| IF expr '{' statement '}'  
	;

elif_stm
	: ELIF expr '{' statement '}' 
	| elif_stm ELIF expr '{' statement '}'
	;

type 
	: HOLLOW
	| INT
	| CHAR
	| FLOAT
	| BOOL
	;
	