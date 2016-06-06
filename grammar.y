%{
#include <stdio.h>
#include "TypeTree/typeTree.h"
#include "SymbolTable/symbolTable.h"
#include "utils/utils.h"

extern int yylineno;
extern char* yytext;

int has_error = 0;

void yyerror (char const *s);
extern int yylex();

Symtable* current = NULL;
Symtable* strings = NULL;
Symtable* helper = NULL;

Typetree* first = NULL;

void constant_string(char*);

void declare_var(char*, Typetree*);
void declare_ptr(char*, int, Typetree*);
void declare_array(char*, Typetree*);
void declare_record(char s_c, char*);
void declare_proc(char*, ArgList*);
void declare_func(char*, ArgList*, Typetree*);

void set_record(char*, Symtable*);

void check_var(char*);
Typetree* check_record(char*);
void check_func(char*);
void check_proc(char*);

%}
%locations
%union {
	int ival;
	float fval;
	char *str;
	char c;
    struct _typetree *type;
    struct _argList *list;
}


%token 	<ival> 	NUMBER
%token 	<fval> 	REAL
%token  <str>   STRING ID SC_ID
%token  <c>     CHARACTER

%token PROGRAM

%token NL

%token PREDEC

%token <str> BOOL CHAR FLOAT INT HOLLOW

%token <c> CONFEDERATION STRUCT

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
    : opt_nls { current = enterScope(current); } definitions nls PROGRAM block opt_nls { current = exitScope(current); }
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
    : init_dcl
    | subrout_def
    | fwd_dec
    ;

block
    : { current = enterScope(current); } '{' opt_nls ins_list opt_nls '}' { current = exitScope(current); }
    ;

ins_list
    : instruction
    | ins_list nls instruction
    ;

instruction
    : init_dcl
    | block
    | selection
    | iteration
    | assignment
    | jump
    | io_inst
    | malloc
    | proc_call
    ;

malloc
    : BORN '(' ID { check_var($3); } ')'
    | PUFF '(' ID { check_var($3); } ')'
    ;

io_inst
    : READ STRING { constant_string($2); } ',' ID { check_var($5); }
	| READ ID { check_var($2); }
    | WRITE STRING { constant_string($2); }
	| WRITE ID { check_var($2); }
	;

jump
	: NEXT
	| BREAK
	| RETURN
	| RETURN expr
	;

declaration
    : type id_list
    | type ID dimension { declare_array($2, first); }
    | type point_d ID { declare_ptr($3, $<ival>2, $<type>1); }
    /* pointer to array y vice versa */
    | s_c SC_ID 
        { 
            declare_record($<c>1, $2); 
            helper = enterScopeR(current, helper); 
        } 
    '{' opt_nls dcl_list opt_nls '}' 
        { 
            set_record($2, helper); 
            helper = exitScope(helper);
            if (helper == current) {
                helper = NULL;
            }
        }
    | s_c SC_ID ID /*{ declare_var($3); }*/
    ;

init_dcl
    : type ID '=' expr { declare_var($2, $<type>1); }
    | type ID dimension '=' expr { declare_array($2, first); }
    | declaration
    ;

id_list
    : ID { declare_var($1, $<type>0); }
    | id_list ',' ID { declare_var($3, $<type>0); }
    ;

dcl_list
    : declaration
    | dcl_list nls declaration
    ;

s_c
    : STRUCT { $<c>$ = 's'; }
    | CONFEDERATION { $<c>$ = 'c'; }
    ;

point_d
    : '*' { $<ival>$ = 1; }
    | point_d '*' { $<ival>$ = $<ival>1 + 1; }
    ;

dimension
    : '[' NUMBER ']' { first = createArray($2, $<type>-1); $<type>$ = first; }
    | dimension '[' NUMBER ']'
        {
            Typetree *t = createArray($3, $<type>-1);
            $<type>1->u.a.t = t;
            $<type>$ = t;
        }
    ;

type
    : HOLLOW    { $<type>$ = lookupTable(current, "hollow", 0)->type; }
    | INT       { $<type>$ = lookupTable(current, "int", 0)->type; }
    | CHAR      { $<type>$ = lookupTable(current, "char", 0)->type; }
    | FLOAT     { $<type>$ = lookupTable(current, "float", 0)->type; }
    | BOOL      { $<type>$ = lookupTable(current, "bool", 0)->type; }
    | SC_ID     { $<type>$ = check_record($1); }
    ;

assignment
    : ID { check_var($1); } '=' expr
    | ID { check_var($1); } PLUS_ASSIGN expr
    | ID { check_var($1); } MINUS_ASSIGN expr
    | ID { check_var($1); } MULT_ASSIGN expr
    | ID { check_var($1); } DIV_ASSIGN expr
    ;

iteration
    : WHILE expr block
    | FOR for_bot_args TO for_args block { current = exitScope(current); }
    | FOR for_bot_args TO for_args STEP NUMBER block { current = exitScope(current); }
    ;

for_bot_args
    : { current = enterScope(current); } for_args

for_args
    : expr
    | init_dcl
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
    | ID { check_var($1); }
    | ID dimension { check_var($1); }
    /* | point_d ID { check_var($2); } */
    | ID '.' ID { check_var($1); /* check_field */ }
    | func_call
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
    : FUNC ID '(' f_formals ')' ARROW type
        { declare_func($2, $<list>4, $<type>7); }
    block
        { current = exitScope(current); }
    | PROC ID '(' p_formals { declare_proc($2, $<list>4); } ')' block { current = exitScope(current); }
    ;

fwd_dec
    : PREDEC FUNC ID '(' f_formals ')' ARROW type
        {
            declare_func($3, $<list>5, $<type>8);
            current = exitScope(current);
        }
    | PREDEC PROC ID '(' p_formals { declare_proc($3, $<list>5); } ')' { current = exitScope(current); }
    ;

f_formals
    : /* lambda */
        {
            current = enterScope(current);
            $<list>$ = newArgList();
        }
    | { current = enterScope(current); } f_formal_list { $<list>$ = $<list>2;}
    ;

p_formals
    : /* lambda */
        {
            current = enterScope(current);
            $<list>$ = newArgList();
        }
    | { current = enterScope(current); } p_formal_list { $<list>$ = $<list>2;}
    ;

f_formal_list
    : f_formal { $<list>$ = add(newArgList(), $<type>1) ; }
    | f_formal_list ',' f_formal { $<list>$ = add($<list>1, $<type>3); }
    ;

p_formal_list
    : p_formal { $<list>$ = add(newArgList(), $<type>1); }
    | p_formal_list ',' p_formal { $<list>$ = add($<list>1, $<type>3); }
    ;

f_formal
    : type ID { declare_var($2, $<type>1); $<type>$ = $<type>1; }
    ;

p_formal
    : type ID { declare_var($2, $<type>1); $<type>$ = $<type>1;  }
    | REF type ID { declare_var($3, $<type>2); $<type>$ = $<type>2; }
    ;

func_call
    : ID { check_func($1); } '(' arguments ')'
    ;

proc_call
    : ID { check_proc($1); } '(' arguments ')'
    ;

arguments
    : /* lambda */
    | args_list
    ;

args_list
    : expr
    | args_list ',' expr
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

void yyerror (char const *s) {
    has_error++;
    fprintf (stderr, "%s %d:%d en %s\n", s, yylineno, yylloc.first_column, yytext);
}

void constant_string(char* str) {
    if(lookupTable(strings, str, 1) == NULL) {
       insertTable(strings, str, yylloc.first_line, yylloc.first_column, lookupTable(current, "hollow", 0)->type);
    }
}

void declare_var(char *id, Typetree *type) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column, type);
    }
    else {
        fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
            yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        has_error = 1;
    }
}

void declare_array(char *id, Typetree *type) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column, type);
    }
    else {
        fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
            yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        has_error = 1;
    }
}

void declare_ptr(char *id, int i, Typetree *type) {
    Entry *aux;
    Typetree *aux_type, *temp;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        aux_type = createType(T_POINTER);
        temp = aux_type;
        for(; i>1; i--) {
            temp->u.p.t = createType(T_POINTER);
            temp = temp->u.p.t;
        }
        temp->u.p.t = type;
        insertTable(current, id, yylloc.first_line, yylloc.first_column, aux_type);
    }
    else {
        fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
            yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        has_error = 1;
    }
}

void declare_record(char s_c, char *id) {
    Entry *aux;
    Typetree *t;
    if(s_c == 's') {
        t = createStruct(id);
    }
    else {
        t = createConf(id);
    }
    if((aux = lookupTable(current, id, 1)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column, t);
    }
    else {
        fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
            yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        has_error = 1;
    }

}

void declare_proc(char *id, ArgList *list) {
    Entry *aux;
    Typetree *t = createProc(list);
    if((aux = lookupTable(current, id, 0)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column, t);
    }
    else {
        if(aux->line) {
            fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarado en linea %d columna %d.\n",
                yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        }
        else {
            fprintf(stderr, "Error:%d:%d: \"%s\" es un procedimiento predefinido por Jaxmin.\n",
                yylloc.first_line, yylloc.first_column, id);
        }
        has_error = 1;
    }
}

void declare_func(char *id, ArgList *l, Typetree *range) {
    printf("%s\n", id);
    Entry *aux;
    Typetree *t = createFunc(l, range);
    if((aux = lookupTable(current, id, 0)) == NULL) {
        printf("%s--%d\n", id, yylloc.first_line);

        insertTable(current, id, yylloc.first_line, yylloc.first_column, t);
    }
    else {
        if(aux->line) {
            fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
                yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        }
        else {
            fprintf(stderr, "Error:%d:%d: \"%s\" es una función predefinida por Jaxmin.\n",
                yylloc.first_line, yylloc.first_column, id);
        }
        has_error = 1;
    }
}

void set_record(char* id, Symtable* table) {
    Entry *aux = lookupTable(current, id, 0);
    Typetree *t = aux->type;
    t->u.r.fields = table;
}


void check_var(char *id) {
    if(lookupTable(current, id, 0) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
}

Typetree* check_record(char* id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
    return aux->type;
}

void check_func(char *id) {
    if(lookupTable(current, id, 0) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
}

void check_proc(char *id) {
    if(lookupTable(current, id, 0) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarado\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
}
