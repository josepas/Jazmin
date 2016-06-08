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
Symtable* save_current = NULL;

Typetree* first = NULL;

Entry *temp = NULL;

void constant_string(char*);

void declare_var(char*, Typetree*);
void declare_ptr(char*, int, Typetree*);
void declare_array(char*, Typetree*);
void declare_record(char s_c, char*);
void declare_proc(char*, ArgList*);
void declare_func(char*, ArgList*, Typetree*);

void set_record(char*, Symtable*);

Entry* check_var(char*);
Entry* check_field(Typetree*, char*);
Typetree* check_record(char*);
Entry* check_func(char*);
Entry* check_proc(char*);

Typetree* check_type(Typetree*, Typetree*);
Typetree* check_type_arit(Typetree*, Typetree*);
Typetree* check_type_bool(Typetree*, Typetree*);
Typetree* check_type_par(Typetree*);
Typetree* check_type_arit_solo(Typetree*);
Typetree* check_type_bool_solo(Typetree*);
Typetree* check_type_record(Typetree*);
void check_type_if(Typetree*);
void check_type_while(Typetree*);
void check_type_assign(char*, Typetree*); 

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

%type <type> expr func_call


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
            save_current = current;
            current = NULL;
            //helper = enterScopeR(current, helper);
            current = enterScopeR(save_current, current);
        }
    '{' opt_nls dcl_list opt_nls '}'
        {
            set_record($2, current);
            current = exitScope(current);
            if (current == save_current) {
                save_current = NULL;
            }
        }
    | s_c SC_ID ID /*{ declare_var($3); }*/
    ;

init_dcl
    : type ID '=' expr { declare_var($2, $<type>1); { check_type_assign($2, $4); }}
    | type ID dimension '=' expr { declare_array($2, first); { check_type_assign($2, $5); }}
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
    : ID { $<type>$ = check_var($1)->type; } '=' expr            { check_type_assign($1, $4); }
    | ID { $<type>$ = check_var($1)->type; } PLUS_ASSIGN expr    { check_type_assign($1, $4); }
    | ID { $<type>$ = check_var($1)->type; } MINUS_ASSIGN expr   { check_type_assign($1, $4); }
    | ID { $<type>$ = check_var($1)->type; } MULT_ASSIGN expr    { check_type_assign($1, $4); }
    | ID { $<type>$ = check_var($1)->type; } DIV_ASSIGN expr     { check_type_assign($1, $4); }
    ;

iteration
    : WHILE expr block { check_type_while($2); }
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
    : IF expr block elif_stm ELSE block { check_type_if($2); }
    | IF expr block elif_stm { check_type_if($2); }
    | IF expr block ELSE block { check_type_if($2); }
    | IF expr block { check_type_if($2); }
    ;

elif_stm
    : ELIF expr { check_type_if($2); } block
    | elif_stm ELIF expr { check_type_if($3); } block
    ;

expr
    : literal   {$<type>$ = $<type>1;}
    | ID
        {
        if((temp = check_var($1)))
            $$ = temp->type;
        }
    | ID dimension
        {
            if((temp = check_var($1)))
                $$ = temp->type;
        }
    /* | point_d ID { check_var($2); } */
    | ID
        {
        if((temp = check_var($1)))
            $<type>$ = check_type_record(temp->type);
        }
    '.' field_id
        {
            $$ = $<type>4;
        }
    | func_call { $$ = $1; }
    | expr '+' expr { $$ = check_type_arit($1, $3); }
    | expr '-' expr { $$ = check_type_arit($1, $3); }
    | expr '*' expr { $$ = check_type_arit($1, $3); }
    | expr '/' expr { $$ = check_type_arit($1, $3); }
    | expr '%' expr { $$ = check_type_arit($1, $3); }
    | '-' expr     { $$ = check_type_arit_solo($2); } %prec UNARY
    | '(' expr ')' { $$ = check_type_par($2); }
    | '~' expr     { $$ = check_type_bool_solo($2); }  %prec UNARY
    | expr AND expr { $$ = check_type_bool($1, $3); }
    | expr OR expr { $$ = check_type_bool($1, $3); }
    | expr LTOE expr
        {
            $$ = check_type_arit($1, $3);
            if($$->kind != T_TYPE_ERROR)
                $$ = lookupTable(current, "bool", 0)->type;
        }
    | expr GTOE expr
        {
            $$ = check_type_arit($1, $3);
            if($$->kind != T_TYPE_ERROR)
                $$ = lookupTable(current, "bool", 0)->type;
        }
    | expr EQUAL expr
        {
            $$ = check_type($1, $3);
            if($$->kind != T_TYPE_ERROR)
                $$ = lookupTable(current, "bool", 0)->type;
        }
    | expr UNEQUAL expr
        {
            $$ = check_type($1, $3);
            if($$->kind != T_TYPE_ERROR)
                $$ = lookupTable(current, "bool", 0)->type;
        }
    | expr '<' expr
        {
            $$ = check_type_arit($1, $3);
            if($$->kind != T_TYPE_ERROR)
                $$ = lookupTable(current, "bool", 0)->type;
        }
    | expr '>' expr
        {
            $$ = check_type_arit($1, $3);
            if($$->kind != T_TYPE_ERROR)
                $$ = lookupTable(current, "bool", 0)->type;
        }
    ;

field_id
    : ID
        {
            if((temp = check_field($<type>-1, $1))) {
                $<type>$ = temp->type;
            }
        }
    | field_id '.' ID
        {
            if((temp = check_field($<type>1, $3))) {
                if(temp->class == C_VAR) {
                    $<type>$ = temp->type;
                }
                else {
                    $<type>$ = check_type_record(temp->type);
                }
            }
        }
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
    : ID
        {
        if((temp = check_func($1)))
            // Por ser mid-rule
            $<type>$ = temp->type->u.fun.range;
        }
    '(' arguments ')'
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
	: NUMBER       { $<type>$ = lookupTable(current, "int", 0)->type; }
	| REAL         { $<type>$ = lookupTable(current, "float", 0)->type; }
	| CHARACTER    { $<type>$ = lookupTable(current, "char", 0)->type; }
	| JTRUE        { $<type>$ = lookupTable(current, "bool", 0)->type; }
	| JFALSE       { $<type>$ = lookupTable(current, "bool", 0)->type; }
	| JNULL        { $<type>$ = lookupTable(current, "hollow", 0)->type; }
	;
%%

void yyerror (char const *s) {
    has_error++;
    fprintf (stderr, "%s %d:%d en %s\n", s, yylineno, yylloc.first_column, yytext);
}

void constant_string(char* str) {
    if(lookupTable(strings, str, 1) == NULL) {
       insertTable(strings, str, yylloc.first_line, yylloc.first_column, C_CONSTANT, lookupTable(current, "hollow", 0)->type);
    }
}

void declare_var(char *id, Typetree *type) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_VAR, type);
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
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_VAR, type);
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
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_VAR, aux_type);
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
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_RECORD, t);
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
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_SUB, t);
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
    Entry *aux;
    Typetree *t = createFunc(l, range);
    if((aux = lookupTable(current, id, 0)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_SUB, t);
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


Entry* check_var(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
    return aux;
}

Entry* check_field(Typetree *record, char *field) {
    Entry *aux;
    if((aux = lookupTable(record->u.r.fields, field, 1)) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no es un campo de %s\n", yylloc.first_line, yylloc.first_column, field, record->u.r.name);
        has_error = 1;
    }
    return aux;
}

Typetree* check_record(char* id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
    return aux->type;
}

Entry* check_func(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
    return aux;
}

Entry* check_proc(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        fprintf(stderr, "Error:%d:%d: \"%s\" no ha sido declarado\n", yylloc.first_line, yylloc.first_column, id);
        has_error = 1;
    }
    return aux;
}

Typetree *check_type(Typetree *t1, Typetree *t2) {
    Typetree *error;
    if(t1->kind == T_TYPE_ERROR)
        return t1;
    if(t2->kind == T_TYPE_ERROR)
        return t2;
    if(t1->kind == t2->kind) {
        if(t1->kind == T_CONF || t1->kind == T_STRUCT) {
            if(strcmp(t1->u.r.name, t2->u.r.name) == 0)
                return t1;
            else {
                error = createType(T_TYPE_ERROR);
                has_error = 1;
            }
        }
        else {
            return t1;
        }

    }
    else {
        error = createType(T_TYPE_ERROR);
        has_error = 1;
    }

    return error;
}

Typetree *check_type_arit(Typetree *t1, Typetree *t2) {
    Typetree *error;
    if(t1->kind == T_TYPE_ERROR)
        return t1;
    if(t2->kind == T_TYPE_ERROR)
        return t2;
    if(t1->kind == t2->kind) {
        if(t1->kind == T_INT || t1->kind == T_FLOAT)
            return t1;
        else {
            error = createType(T_TYPE_ERROR);
            has_error = 1;
        }
    }
    else {
        error = createType(T_TYPE_ERROR);
        has_error = 1;
    }

    return error;
}

Typetree *check_type_bool(Typetree *t1, Typetree *t2) {
    Typetree *error;
    if(t1->kind == T_TYPE_ERROR)
        return t1;
    if(t2->kind == T_TYPE_ERROR)
        return t2;
    if(t1->kind == t2->kind) {
        if(t1->kind == T_BOOL)
            return t1;
        else {
            error = createType(T_TYPE_ERROR);
            has_error = 1;
        }
    }
    else {
        error = createType(T_TYPE_ERROR);
        has_error = 1;
    }

    return error;
}

Typetree* check_type_par(Typetree* t) {
    Typetree *error;
    if(t->kind == T_TYPE_ERROR)
        return t;

    if(t->kind == T_INT || t->kind == T_FLOAT || t->kind == T_BOOL) {
        return t;
    }
    else {
        error = createType(T_TYPE_ERROR);
        has_error = 1;
    }

    return error;
}

Typetree* check_type_arit_solo(Typetree* t) {
    Typetree *error;
    if(t->kind == T_TYPE_ERROR)
        return t;

    if(t->kind == T_INT || t->kind == T_FLOAT) {
        return t;
    }
    else {
        error = createType(T_TYPE_ERROR);
        has_error = 1;
    }

    return error;
}

Typetree* check_type_bool_solo(Typetree* t) {
    Typetree *error;
    if(t->kind == T_TYPE_ERROR)
        return t;

    if(t->kind == T_BOOL) {
        return t;
    }
    else {
        error = createType(T_TYPE_ERROR);
        has_error = 1;
    }

    return error;
}

void check_type_if(Typetree* t) {
    if(t->kind != T_BOOL) {
        fprintf(stderr, "Error:%d:%d: expresión del if no booleana\n", yylloc.first_line, yylloc.first_column);
        has_error = 1;
    }
}

void check_type_while(Typetree* t) {
    if(t->kind != T_BOOL) {
        fprintf(stderr, "Error: expresión del while no booleana\n");
        has_error = 1;
    }
}


void check_type_assign(char* id, Typetree *t2) {

     Typetree* t1 = lookupTable(current, id, 0)->type;

    if (t2->kind == T_TYPE_ERROR) {
        fprintf(stderr, "Error: asignación invalida\n");
    } else {
        if (t1->kind != t2->kind) {
            // falta arreglar aqui
            printf("Error: asignación: se espera un ");
            dumpType(t1);
            printf(" y se recibió un ");
            dumpType(t2);
            printf("\n");   
        }
    }
}

Typetree* check_type_record(Typetree *t) {
    Typetree *error;
    if(t->kind == T_TYPE_ERROR)
        return t;

    if(t->kind == T_STRUCT || t->kind == T_CONF)
        return t;
    else {
        error = createType(T_TYPE_ERROR);
        has_error = 1;
    }

    return error;
}
