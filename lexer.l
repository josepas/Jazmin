%{

%}

%option yylineno
%option noyywrap


ID			[:alpha:][[:alnum:]_]*
NUMBER 		[+\-][:digit:]+
FLOAT  		[+\-][:digit:]+\.[:digit:]+


%%


[ \t]+   			/* Nada */
\n 					return NL;
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


<=		return
>=		return
==		return
!=		return
+=		return
*=		return
/=		return
-=		return
/\		return
\/		return
~		return
->		return
#.*\n   /* Nada */
#>		return
<#		return
>		return
[		return
]		return
{		return
}		return
(		return
)		return
\.		return
=		return
+		return
-		return
*		return
/		return
%		return
-		return
<		return

.     	return

%%
