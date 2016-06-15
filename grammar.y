%{
#include <stdio.h>
#include "TypeTree/typeTree.h"
#include "SymbolTable/symbolTable.h"
#include "utils/utils.h"

extern int yylineno;
extern char* yytext;

int has_error = 0;
int offset = 0;
Offsetstack *offstack = NULL;

void yyerror (char const *s);
extern int yylex();

Symtable* current = NULL;
Symtable* strings = NULL;
Symtable* save_current = NULL;

Typetree* first = NULL;

Entry *temp = NULL;
Entry *entry = NULL;

void constant_string(char*);

void declare_var(char*, Typetree*);
void declare_ptr(char*, int, Typetree*);
void declare_array(char*, Typetree*);
void declare_record(char s_c, char*);
void declare_proc(char*, ArgList*);
void declare_func(char*);

void set_type_func(char*, ArgList*, Typetree*);
void set_record(char*, Symtable*);

Typetree* get_array_type(Typetree*);
int count_array_dim(Typetree*);
void check_array_dims(Typetree*, Typetree*);

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
Typetree* check_type_int(Typetree*);
Typetree* check_type_bool_solo(Typetree*);
Typetree* check_type_record(Typetree*);

void check_type_if(Typetree*);
void check_type_while(Typetree*);
void check_type_assign(Typetree*, Typetree*);

void check_arguments(char*, ArgList*);

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

%type <type> expr sub_call


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
    : opt_nls { offstack = createStack(); current = enterScope(current); } definitions nls PROGRAM block opt_nls { current->size = offset; offset = pop(offstack); current = exitScope(current); }
    | opt_nls { offstack = createStack(); } PROGRAM block opt_nls
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
    : { current = enterScope(current); push(offstack, offset); offset = 0; } '{' opt_nls ins_list opt_nls '}' { current->size = offset; offset = pop(offstack); current = exitScope(current); }
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
    | sub_call
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
            push(offstack, offset); offset = 0;
            declare_record($<c>1, $2);
            save_current = current;
            current = NULL;
            current = enterScopeR(save_current, current);
        }
    '{' opt_nls dcl_list opt_nls '}'
        {
            set_record($2, current);
            current->size = offset;
offset = pop(offstack);             current = exitScope(current);
            if (current == save_current) {
                save_current = NULL;
            }
        }
    | s_c SC_ID ID /*{ declare_var($3); }*/
    ;

init_dcl
    : type ID '=' expr { declare_var($2, $<type>1); { check_type_assign( lookupTable(current, $2, 0)->type , $4); }}
    | type ID dimension '=' expr { declare_array($2, first); { check_type_assign( lookupTable(current, $2, 0)->type, $5); }}
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
    : '[' NUMBER ']' { first = createArray($2, $<type>-1); $<type>$ = first;}
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
    : ID { $<type>$ = check_var($1)->type; } '=' expr            { check_type_assign( lookupTable(current, $1, 0)->type, $4); }
    | ID { $<type>$ = check_var($1)->type; } PLUS_ASSIGN expr    { check_type_assign( lookupTable(current, $1, 0)->type, $4); }
    | ID { $<type>$ = check_var($1)->type; } MINUS_ASSIGN expr   { check_type_assign( lookupTable(current, $1, 0)->type, $4); }
    | ID { $<type>$ = check_var($1)->type; } MULT_ASSIGN expr    { check_type_assign( lookupTable(current, $1, 0)->type, $4); }
    | ID { $<type>$ = check_var($1)->type; } DIV_ASSIGN expr     { check_type_assign( lookupTable(current, $1, 0)->type, $4); }
    | ID dimension
        {
            check_array_dims( lookupTable(current, $1, 0)->type, first );
        }
     '=' expr
        {
            check_type_assign( get_array_type( lookupTable(current, $1, 0)->type ), $5  );
        }
    ;

iteration
    : WHILE expr block { check_type_while($2); }
    | FOR for_bot_args TO for_args block { current->size = offset; offset = pop(offstack); current = exitScope(current); }
    | FOR for_bot_args TO for_args STEP NUMBER block { current->size = offset; offset = pop(offstack); current = exitScope(current); }
    ;

for_bot_args
    : { current = enterScope(current); push(offstack, offset); offset = 0; } for_args

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
    | sub_call { $$ = $1; }
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
    : FUNC ID { declare_func($2); } '(' f_formals ')' ARROW type
        { set_type_func($2, $<list>5, $<type>8); }
    block
        { current->size = offset; offset = pop(offstack); current = exitScope(current); }
    | PROC ID '(' p_formals { declare_proc($2, $<list>4); } ')' block { current->size = offset; offset = pop(offstack); current = exitScope(current); }
    ;

fwd_dec
    : PREDEC FUNC ID { declare_func($3); } '(' f_formals ')' ARROW type
        {
            //declare_func($3);
            current->size = offset;
offset = pop(offstack);             current = exitScope(current);
        }
    | PREDEC PROC ID '(' p_formals { declare_proc($3, $<list>5); } ')' { current->size = offset; offset = pop(offstack); current = exitScope(current); }
    ;

f_formals
    : /* lambda */
        {
            current = enterScope(current);
            push(offstack, offset); offset = 0;
            $<list>$ = newArgList();
        }
    | { current = enterScope(current); push(offstack, offset); offset = 0; } f_formal_list { $<list>$ = $<list>2;}
    ;

p_formals
    : /* lambda */
        {
            current = enterScope(current);
            push(offstack, offset); offset = 0;
            $<list>$ = newArgList();
        }
    | { current = enterScope(current); push(offstack, offset); offset = 0; } p_formal_list { $<list>$ = $<list>2;}
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

sub_call
    : ID
        {
        if((temp = check_func($1)))
            // Por ser mid-rule
            $<type>$ = temp->type->u.fun.range;
        }
    '(' arguments ')'
        {
            check_arguments($1, $<list>4);
            $<type>$ = $<type>1->u.fun.range;
        }
    ;

arguments
    : /* lambda */  { $<list>$ = newArgList(); }
    | args_list     { $<list>$ = $<list>1; }
    ;

args_list
    : expr
        {
            $<list>$ = add( newArgList(), $<type>1 );
        }
    | args_list ',' expr
        {
            $<list>$ = add($<list>1, $<type>3);
        }
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
        // offset de strings ?
        insertTable(strings, str, yylloc.first_line, yylloc.first_column, C_CONSTANT, lookupTable(current, "hollow", 0)->type, 0, 0);
    }
}

void declare_var(char *id, Typetree *type) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_VAR, type, type->size, offset);
        offset += type->size;
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
        Typetree *temp = type;
        int size = 1;
        while(temp->kind == T_ARRAY) {
            size = size * (temp->u.a.high - temp->u.a.low + 1);
            temp = temp->u.a.t;
        }
        size = size * temp->size;
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_ARRAY, type, size, offset);
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
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_VAR, aux_type, 4, offset);
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
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_RECORD, t, 0, offset);
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
        int size = 0; //cambiar
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_SUB, t, size, offset);
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

void declare_func(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        int size = 0; //cambiar
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_SUB, NULL, size, offset);
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

void set_type_func(char* id, ArgList *list, Typetree *range) {
    Entry *aux = lookupTable(current,id,0);
    aux->type = createFunc(list, range);
}



void set_record(char* id, Symtable* table) {
    Entry *aux = lookupTable(current, id, 0);
    Typetree *t = aux->type;
    t->u.r.fields = table;
    t->size = table->size;
}

Typetree* get_array_type(Typetree* t) {

    if (t == NULL) {
        fprintf(stderr, "Fallo miserable en el chequeo de tipos de arreglos\n");
    }
    if (t->kind != T_ARRAY) {
        return t;
    }
    return get_array_type(t->u.a.t);
}

int count_array_dim(Typetree* t) {
    if (t->kind != T_ARRAY) {
        return 0;
    }
    return 1 + count_array_dim(t->u.a.t);
}

void check_array_dims(Typetree *a1, Typetree *a2) {
    int d1 = count_array_dim(a1);
    int d2 = count_array_dim(a2);

    if (d1 != d2) {
        fprintf(stderr, "Error:%d:%d: asignación de arreglo mal formada", yylloc.first_line, yylloc.first_column);
    }
}


Entry* check_var(char *id) {
    Entry *aux;
    if((aux = lookupTable(current, id, 0)) == NULL) {
        fprintf(stderr, "Error:%d:%d: variable \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
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
        fprintf(stderr, "Error:%d:%d: subrutina \"%s\" no ha sido declarada\n", yylloc.first_line, yylloc.first_column, id);
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

Typetree* check_type_int(Typetree* t) {
    Typetree *error;
    if(t->kind == T_TYPE_ERROR)
        return t;

    if(t->kind == T_INT) {
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


void check_type_assign(Typetree *t1, Typetree *t2) {

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

void check_arguments(char *id, ArgList *l2) {

    TypeNode *n1;
    TypeNode *n2;

    Typetree *t = lookupTable(current, id, 0)->type;

    if ( t->kind == T_FUNC ) {
        n1 = t->u.fun.dom->f;

    }  else if (t->kind == T_PROC ) {
        n1 = t->u.proc.dom->f;

    } else {
        fprintf(stderr, "FATAL: Fallo en el chequeo de tipos\n");
    }

    n2 = l2->f;

    if ( !compareTypeNodes(n1,n2) ) {
        fprintf(stderr, "Error: Tipos en la llamada de la funcion\n");
    }


}

