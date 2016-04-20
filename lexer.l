%{
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 
#d
%}


%%

bool				return BOOL;
born				return BORN;
break				return BREAK;
char				return CHAR;
confederation		return CONFEDERATION;
elif				return ELIF;
else				return ELSE;
False				return FALSE;
float				return FLOAT;
for					return FOR;
func				return FUNC;
hollow				return HOLLOW;
if					return IF;
int					return INT;
next				return NEXT;
Null				return NULL;
proc				return PROC;
puff				return PUFF;
read				return READ;
ref					return REF;
return				return RETURN;
step				return STEP;
struct				return STRUCT;
to					return TO;
True				return TRUE;
while				return WHILE;
write				return WRITE;





[ \t\n]         ;
[0-9]+\.[0-9]+  { cout << "Found a floating-point number:" << yytext << endl; }
[0-9]+          { cout << "Found an integer:" << yytext << endl; }
[a-zA-Z0-9]+    { cout << "Found a string: " << yytext << endl; }


%%
