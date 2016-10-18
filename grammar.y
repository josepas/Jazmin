%{
#include <stdio.h>
#include "TypeTree/typeTree.h"
#include "SymbolTable/symbolTable.h"
#include "AST/ast.h"
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
Typetree* HOLLOW_T;

AST *tree = NULL;

Entry* constant_string(char*);

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

Typetree* check_type_if(Typetree*, Typetree*);
Typetree* check_type_while(Typetree*, Typetree*);
Typetree* check_for(Typetree*, Typetree*, Typetree*);
Typetree* check_type_assign(Typetree*, Typetree*);
Typetree* check_arguments(char*, ArgList*);

Typetree* check_seq(Typetree*, Typetree*);
Typetree* check_block(Typetree*);
Typetree* check_program(Typetree*);

AST* set_node_type(AST*, Typetree*);
void set_subrout_ast(char*, AST*);

void set_last_dims_expr(AST*, AST*);
%}
%locations
%union {
	int ival;
	float fval;
	char *str;
	char c;
    struct _typetree *type;
    struct _argList *list;
    struct _ast *node;
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

%type <node> expr sub_call


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
    : opt_nls
        {
            offstack = createStack();
            current = enterScope(current);
        }
    definitions nls PROGRAM block opt_nls
        {
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            tree = set_node_type(
                newProgramNode($<node>3, $<node>6),
                //check_program($<node>4->type)
                check_seq($<node>3->type, $<node>6->type)
                );
            //dumpAST(tree,1);
        }
    | opt_nls { printf("debug"); offstack = createStack(); } PROGRAM block opt_nls
        {
            tree = set_node_type(
                newProgramNode(NULL, $<node>4),
                check_program($<node>4->type)
                );
            //dumpAST(tree,1);
        }
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
        {
            $<node>$ = newSeqNode();
            if($<node>1 != NULL) {
                addASTChild($<node>$, $<node>1);
                $<node>$->type = $<node>1->type;
            } else
                $<node>$->type = HOLLOW_T;
        }
    | definitions nls outer_def
        {
            if($<node>3 != NULL) {
                $<node>$ = set_node_type(
                    $<node>1,
                    check_seq($<node>1->type, $<node>3->type)
                    );
                addASTChild($<node>$, $<node>3);
            }
        }
	;

outer_def
    : init_dcl      { $<node>$ = $<node>1; }
    | subrout_def   { $<node>$ = $<node>1; }
    | fwd_dec       { $<node>$ = $<node>1; }
    ;

block
    :   {
            current = enterScope(current);
            push(offstack, offset);
            offset = 0;
        }
     '{' opt_nls ins_list opt_nls '}'
        {
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            $<node>$ = set_node_type($<node>4, check_block($<node>4->type));
        }
    ;

ins_list
    : instruction
        {
            $<node>$ = newSeqNode();
            if($<node>1 != NULL) {
                addASTChild($<node>$, $<node>1);
                $<node>$->type = $<node>1->type;
            } else {
                $<node>$->type = HOLLOW_T;
            }
            //$<node>$ = set_node_type(addASTChild(newSeqNode(), $<node>1), $<node>1->type);
        }
    | ins_list nls instruction
        {
            if($<node>3 != NULL) {
                $<node>$ = set_node_type(
                    $<node>1,
                    check_seq($<node>1->type, $<node>3->type)
                    );
                addASTChild($<node>$, $<node>3);
            }
        }
    ;

instruction
    : init_dcl        { $<node>$ = $<node>1; }
    | block           { $<node>$ = $<node>1; }
    | selection       { $<node>$ = $<node>1; }
    | iteration       { $<node>$ = $<node>1; }
    | assignment      { $<node>$ = $<node>1; }
    | jump            { $<node>$ = $<node>1; }
    | io_inst         { $<node>$ = $<node>1; }
    | malloc          { $<node>$ = $<node>1; }
    | sub_call        { $<node>$ = $<node>1; }
    ;

malloc
    : BORN '(' ID { temp = check_var($3); } ')'
        {
            $<node>$ = newBornNode( newVarNode(temp) );
        }
    | PUFF '(' ID { temp = check_var($3); } ')'
        {
            $<node>$ = newPuffNode( newVarNode(temp) );
        }
    ;

io_inst
    : READ STRING { temp = constant_string($2); } ',' ID
        {
            $<node>$ = newReadNode(
                temp,
                newVarNode( check_var($5) )
            );
            $<node>$->type = HOLLOW_T;
        }

	| READ ID
        {
            $<node>$ = newReadNode(
                NULL,
                newVarNode( check_var($2) )
            );
            $<node>$->type = HOLLOW_T;
        }

    | WRITE STRING { temp = constant_string($2); }
        {
            $<node>$ = newWriteNode(
                temp,
                NULL
            );
            $<node>$->type = HOLLOW_T;
        }

	| WRITE ID
        {
            $<node>$ = newWriteNode(
                NULL,
                newVarNode( check_var($2) )
            );
            $<node>$->type = HOLLOW_T;
        }
	;

jump
	: NEXT             { $<node>$ = newNextNode(); }
	| BREAK            { $<node>$ = newBreakNode(); }
	| RETURN           { $<node>$ = newReturnNode(NULL); $<node>$->type = HOLLOW_T; }
	| RETURN expr      { $<node>$ = newReturnNode($2); $<node>$->type = $2->type; }
	;

declaration
    : type id_list { $<node>$ = $<node>2; }
    | type ID dimension
        {
            declare_array($2, first);
            if($<type>1->kind == T_STRUCT || $<type>1->kind == T_CONF)
                $<node>$ = NULL;
            else {
                temp = lookupTable(current, $2, 1);
                $<node>$ = set_node_type(
                    newAssignNode(newVarNode(temp), "=", newBaseTypeNode($<type>1->kind)),
                    $<type>1
                    );
            }
        }
    | type point_d ID { declare_ptr($3, $<ival>2, $<type>1); $<node>$ = newPtrNode( lookupTable(current, $3, 1) ); }
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
            offset = pop(offstack);
            current = exitScope(current);
            if (current == save_current) {
                save_current = NULL;
            }
            //$<type>$ = lookupTable(current, $2, 1)->type;
            $<node>$ = NULL;
        }
    ;

init_dcl
    : type ID '=' expr
        {
            declare_var($2, $<type>1);
            Entry *aux = lookupTable(current, $2, 1);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(aux), "=", $4),
                check_type_assign(aux->type , $4->type)
                );
        }
    | type ID dimension '=' expr
        {
            declare_array($2, first);
            Entry *aux = lookupTable(current, $2, 1);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(aux), "=", $5),
                check_type_assign(aux->type, $5->type)
                );
        }
    | declaration { $<node>$ = $<node>1;} // se puede cambiar esto luego
    ;

id_list
    : ID
        {
            declare_var($1, $<type>0);
            temp = lookupTable(current, $1, 1);
            if($<type>0->kind == T_STRUCT || $<type>0->kind == T_CONF)
                $<node>$ = NULL;
            else {
                $<node>$ = set_node_type(
                addASTChild(
                    newSeqNode(),
                    newAssignNode(newVarNode(temp), "=", newBaseTypeNode($<type>0->kind))
                    ),
                $<type>0
                );
            }
        }
    | id_list ',' ID
        {
            declare_var($3, $<type>0);
            if($<type>0->kind == T_STRUCT || $<type>0->kind == T_CONF)
                $<node>$ = NULL;
            else {
                temp = lookupTable(current, $3, 1);
                $<node>$ = addASTChild(
                    $<node>1,
                    newAssignNode(newVarNode(temp), "=", newBaseTypeNode($<type>0->kind))
                    );
                set_node_type($<node>$->last, $<type>0);
            }
        }
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

dims_expr
    : '[' expr ']'
        {
            $2->type = check_type_int($2->type);
            $2->next = NULL;
            $<node>$ = $2;
        }
    | dims_expr '[' expr ']'
        {
            $3->type = check_type_int($3->type);
            set_last_dims_expr($<node>1, $3);
            $<node>$ = $<node>1;
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

assignment // refactorizar
    : ID { check_var($1); } '=' expr
        {
            temp = lookupTable(current, $1, 0);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(temp), "=", $4),
                check_type_assign(temp->type, $4->type)
                );
        }
    | ID { check_var($1); } PLUS_ASSIGN expr
        {
            temp = lookupTable(current, $1, 0);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(temp), "+=", $4),
                check_type_assign(temp->type, $4->type)
                );
        }
    | ID { check_var($1); } MINUS_ASSIGN expr
        {
            temp = lookupTable(current, $1, 0);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(temp), "-=", $4),
                check_type_assign(temp->type, $4->type)
                );
        }
    | ID { check_var($1); } MULT_ASSIGN expr
        {
            temp = lookupTable(current, $1, 0);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(temp), "*=", $4),
                check_type_assign(temp->type, $4->type)
                );
        }
    | ID { check_var($1); } DIV_ASSIGN expr
        {
            temp = lookupTable(current, $1, 0);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(temp), "/=", $4),
                check_type_assign(temp->type, $4->type)
                );
        }
    | ID dims_expr
        {
            check_array_dims( lookupTable(current, $1, 0)->type, first );
        }
     '=' expr
        {
            temp = lookupTable(current, $1, 0);
            $<node>$ = set_node_type(
                newAssignNode(newVarNode(temp), "=", $5),
                check_type_assign(get_array_type(temp->type), $5->type)
                );
            $<node>$->first->first = $<node>2;
        }
    | ID
        {
        if((temp = check_var($1)))
            $<type>$ = check_type_record(temp->type);
        }
    '.' field_id '=' expr
        {
            $<node>$ = set_node_type(
                newAssignNode($<node>4, "=", $6),
                check_type_assign($<node>4->type, $6->type)
                );
        }
    ;

iteration // permitiremos declaraciones solas en el for?
    : WHILE expr block
        {
            $<node>$ = set_node_type(
                newWhileNode($2, $<node>3),
                check_type_while($2->type, $<node>3->type));
        }
    | FOR for_bot_args TO for_args block
        {
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            $<node>$ = set_node_type(
                newForNode($<node>2, $<node>4, NULL, $<node>5),
                check_for($<node>2->type, $<node>4->type, $<node>5->type));
        }
    | FOR for_bot_args TO for_args STEP NUMBER block
        {
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            $<node>$ = set_node_type(
                newForNode($<node>2, $<node>4, newIntNode($6), $<node>7),
                check_for($<node>2->type, $<node>4->type, $<node>7->type));
        }
    ;

for_bot_args
    :   {
            current = enterScope(current);
            push(offstack, offset);
            offset = 0;
        }
     for_args
        {
            $<node>$ = $<node>2;
        }
    ;

for_args
    : expr      {$<node>$ = $<node>1;}
    | init_dcl  {$<node>$ = $<node>1;}

    ;

selection
    : IF expr block elif_stm ELSE block
        {
            $<node>$ = set_node_type(
                newIfNode($2, $<node>3, $<node>4, $<node>6),
                check_type_if($2->type, $<node>3->type)
                );
        }
    | IF expr block elif_stm
        {
            $<node>$ = set_node_type(
                newIfNode($2, $<node>3, $<node>4, NULL),
                check_type_if($2->type, $<node>3->type)
                );
        }
    | IF expr block ELSE block
        {
            $<node>$ = set_node_type(
                newIfNode($2, $<node>3, NULL, $<node>5),
                check_type_if($2->type, $<node>3->type)
                );
        }
    | IF expr block
        {
            $<node>$ = set_node_type(
                newIfNode($2, $<node>3, NULL, NULL),
                check_type_if($2->type, $<node>3->type)
                );
        }
    ;

elif_stm
    : ELIF expr block
        {
            $<node>$ = set_node_type(
                newIfNode($2, $<node>3, NULL, NULL),
                check_type_if($2->type, $<node>3->type)
                );
        }
    | elif_stm ELIF expr block
        {
            $<node>$ = set_node_type(
                $<node>1,
                check_type_if($3->type, $<node>4->type)
                );
            addASTChild($<node>$, $3);
            addASTChild($<node>$, $<node>4);
        }
    ;

expr
    : literal   { $$ = $<node>1;}
    | ID
        {
        if((temp = check_var($1)))
            $$ = newVarNode(temp);
        }
    | ID dims_expr
        {
            if((temp = check_var($1))) {
                $$ = newVarNode(temp);
                $$->first = $<node>2;
            }
        }
    /* | point_d ID { check_var($2); } */
    | ID
        {
            if((temp = check_var($1)))
                $<type>$ = check_type_record(temp->type);
        }
    '.' field_id
        {
            $$ = $<node>4;
        }
    | sub_call { $$ = $<node>1; }
    | expr '+' expr
        {
            $$ = set_node_type(
                newBinOp($1,"+",$3),
                check_type_arit($1->type, $3->type)
                );
        }
    | expr '-' expr
        {
            $$ = set_node_type(
                newBinOp($1,"-",$3),
                check_type_arit($1->type, $3->type)
                );
        }
    | expr '*' expr
        {
            $$ = set_node_type(
                newBinOp($1,"*",$3),
                check_type_arit($1->type, $3->type)
                );
        }
    | expr '/' expr
        {
            $$ = set_node_type(
                newBinOp($1,"/",$3),
                check_type_arit($1->type, $3->type)
                );
        }
    | expr '%' expr
        {
            $$ = set_node_type(
                newBinOp($1,"%",$3),
                check_type_arit($1->type, $3->type)
                );
        }
    | '-' expr
        { $$ = set_node_type(
            newUnaryOp("-", $2),
            check_type_arit_solo($2->type)
            );
        } %prec UNARY
    | '(' expr ')' { $$ = $2; }
    | '~' expr
        {
            $$ = set_node_type(
                newUnaryOp("~", $2),
                check_type_bool_solo($2->type)
                );
        }  %prec UNARY
    | expr AND expr
        {
            $$ = set_node_type(
                newBinOp($1, "/\\", $3),
                check_type_bool($1->type, $3->type)
                );
        }
    | expr OR expr
        {
            $$ = set_node_type(
                newBinOp($1, "\\/", $3),
                check_type_bool($1->type, $3->type)
                );
        }
    | expr LTOE expr
        {
            $$ = set_node_type(
                    newBinOp($1,"<=",$3),
                    check_type_arit($1->type, $3->type)
                );
            if($$->tag != N_ERROR)
                $$->type = lookupTable(current, "bool", 0)->type;
        }
    | expr GTOE expr
        {
            $$ = set_node_type(
                    newBinOp($1,">=",$3),
                    check_type_arit($1->type, $3->type)
                );
            if($$->tag != N_ERROR)
                $$->type = lookupTable(current, "bool", 0)->type;
        }
    | expr '<' expr
        {
            $$ = set_node_type(
                    newBinOp($1,"<",$3),
                    check_type_arit($1->type, $3->type)
                );
            if($$->tag != N_ERROR)
                $$->type = lookupTable(current, "bool", 0)->type;
        }
    | expr '>' expr
        {
            $$ = set_node_type(
                    newBinOp($1,">",$3),
                    check_type_arit($1->type, $3->type)
                );
            if($$->tag != N_ERROR)
                $$->type = lookupTable(current, "bool", 0)->type;
        }
    | expr EQUAL expr
        {
            $$ = set_node_type(
                    newBinOp($1,"==",$3),
                    check_type($1->type, $3->type)
                );
            if($$->tag != N_ERROR && $$->type->kind != T_BOOL)
                $$->type = lookupTable(current, "bool", 0)->type;
        }
    | expr UNEQUAL expr
        {
            $$ = set_node_type(
                    newBinOp($1,"!=",$3),
                    check_type($1->type, $3->type)
                );
            if($$->tag != N_ERROR && $$->type->kind != T_BOOL)
                $$->type = lookupTable(current, "bool", 0)->type;
        }
    ;

field_id
    : ID
        {
            if((temp = check_field($<type>-1, $1))) {
                $<node>$ = newVarNode(temp);
            }
        }
    | field_id '.' ID
        {
            if((temp = check_field($<node>1->type, $3))) {
                if(temp->class == C_VAR) {
                    $<node>$ = $<node>1;
                    $<node>$->u.sym = temp;
                    $<node>$->type = temp->type;
                }
                else {
                    $<node>$ = set_node_type($<node>1, check_type_record(temp->type));
                }
            }
        }
    ;

subrout_def
    : FUNC ID { declare_func($2); } '(' f_formals ')' ARROW type
        { set_type_func($2, $<list>5, $<type>8); }
    block
        {
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            set_subrout_ast($2, $<node>10);
            $<node>$ = NULL;
        }
    | PROC ID '(' p_formals { declare_proc($2, $<list>4); } ')' block
        {
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            set_subrout_ast($2, $<node>7);
            $<node>$ = NULL;
        }
    ;

fwd_dec
    : PREDEC FUNC ID { declare_func($3); } '(' f_formals ')' ARROW type
        {
            //declare_func($3);
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            $<node>$ = NULL;
        }
    | PREDEC PROC ID '(' p_formals { declare_proc($3, $<list>5); } ')'
        {
            current->size = offset;
            offset = pop(offstack);
            current = exitScope(current);
            $<node>$ = NULL;
        }
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
            $<node>$ = set_node_type(
                lookupTable(current, $1, 0)->ast,
                check_arguments($1, $<list>4)
                );
        }
    ;

arguments
    : /* lambda */  { $<list>$ = newArgList(); }
    | args_list     { $<list>$ = $<list>1; }
    ;

args_list
    : expr
        {
            $<list>$ = add( newArgList(), $<node>1->type );
        }
    | args_list ',' expr
        {
            $<list>$ = add($<list>1, $<node>3->type);
        }
    ;

literal
	: NUMBER       { $<node>$ = newIntNode($1); }
	| REAL         { $<node>$ = newFloatNode($1); }
	| CHARACTER    { $<node>$ = newCharNode($1); }
	| JTRUE        { $<node>$ = newBoolNode(1); }
	| JFALSE       { $<node>$ = newBoolNode(0); }
	| JNULL        { }
	;
%%

void yyerror (char const *s) {
    has_error++;
    fprintf (stderr, "%s %d:%d en %s\n", s, yylineno, yylloc.first_column, yytext);
}

Entry* constant_string(char* str) {
    if(lookupTable(strings, str, 1) == NULL) {
        // offset de strings ?
        insertTable(strings, str, yylloc.first_line, yylloc.first_column, C_CONSTANT, lookupTable(current, "hollow", 0)->type, 0, 0);
    }
    return lookupTable(strings, str, 1);
}

void declare_var(char *id, Typetree *type) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        if(type->kind == T_STRUCT || type->kind == T_CONF) {
            if(offset % 4 != 0) {
                offset += 4 - (offset % 4);
           }
        }
        else if(offset % type->size != 0) {
            offset += type->size - (offset % type->size);
        }

        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_VAR, type, type->size, offset);
        offset += type->size;
    }
    else {
        fprintf(stderr, "Error:%d:%d: \"%s\" ya fue declarada en linea %d columna %d.\n",
            yylloc.first_line, yylloc.first_column, id, aux->line, aux->column);
        has_error = 1;
    }
}

int get_and_set_array_size(Typetree *type) {
    if(type->kind == T_ARRAY)
        type->size = get_and_set_array_size(type->u.a.t) * (type->u.a.high - type->u.a.low + 1);

    return type->size;
}

void declare_array(char *id, Typetree *type) {
    Entry *aux;
    if((aux = lookupTable(current, id, 1)) == NULL) {
        //Typetree *temp = type;
        int size = 1;
        /*while(temp->kind == T_ARRAY) {
            size = size * (temp->u.a.high - temp->u.a.low + 1);
            temp = temp->u.a.t;
        }
        */
        size = get_and_set_array_size(type);
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_ARRAY, type, size, offset);
        offset += size;
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
        insertTable(current, id, yylloc.first_line, yylloc.first_column, C_VAR, aux_type, aux_type->size, offset);
        offset += aux_type->size;
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

    if(t->kind == T_STRUCT) {
        t->size = table->size;
        if(t->size % 4 != 0)
            t->size += 4 - (t->size % 4);
    } else {
        t->size = getMaxSize(table);
        if(t->size % 4 != 0)
            t->size += 4 - (t->size % 4);
    }
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

    Typetree *temp1 = t1;
    if(temp1->kind == T_ARRAY)
        temp1 = get_array_type(temp1);

    Typetree *temp2 = t2;
    if(temp2->kind == T_ARRAY)
        temp2 = get_array_type(temp2);


    if(temp1->kind == temp2->kind) {
        if(temp1->kind == T_INT || temp1->kind == T_FLOAT)
            return temp1;
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

Typetree* check_type_if(Typetree* t, Typetree *blockT) {

    if(t->kind != T_BOOL) {
        fprintf(stderr, "Error:%d:%d: expresión del if no booleana\n", yylloc.first_line, yylloc.first_column);
        has_error = 1; // quitar luego
        return createType(T_TYPE_ERROR);
    }
    if (blockT->kind == T_TYPE_ERROR) {
        return blockT;
    }
    return HOLLOW_T;

}

Typetree* check_type_while(Typetree* t, Typetree *blockT) {

    if(t->kind != T_BOOL) {
        fprintf(stderr, "Error: expresión del while no booleana\n");
        has_error = 1; //quitar luego
        return createType(T_TYPE_ERROR);
    }
    if (blockT->kind == T_TYPE_ERROR) {
        return blockT;
    }
    return HOLLOW_T;
}

Typetree* check_for(Typetree *start, Typetree *end, Typetree *block) {

    if  ( check_type_int(start)->kind == T_TYPE_ERROR) {
        fprintf(stderr, "Error en valor inicial del for\n");
        return createType(T_TYPE_ERROR);
    }
    if  ( check_type_int(end)->kind == T_TYPE_ERROR) {
        fprintf(stderr, "Error en valor final del for\n");
        return createType(T_TYPE_ERROR);
    }
    if (block->kind == T_TYPE_ERROR) {
        return block;
    }
    return HOLLOW_T;
}


Typetree* check_type_assign(Typetree *t1, Typetree *t2) {

    if (t2->kind == T_TYPE_ERROR) {
        fprintf(stderr, "Error: asignación invalida\n");
        return t2;
    } else {
        Typetree *temp = t2;
        if (temp->kind == T_ARRAY)
            temp = get_array_type(t2);
        if (t1->kind != temp->kind) {
            // falta arreglar aqui
            printf("Error: asignación: se espera un \"");
            dumpType(t1);
            printf("\" y se recibió un \"");
            dumpType(temp);
            printf("\"\n");
            return createType(T_TYPE_ERROR);
        }
    }
    return t1;
}

Typetree* check_seq(Typetree *t1, Typetree *t2) {

    if ( t1->kind == T_TYPE_ERROR ) {
        return t1;
    }
    if ( t2->kind == T_TYPE_ERROR ) {
        return t2;
    }
    return HOLLOW_T;

}

Typetree* check_block(Typetree *t1) {

    if (t1->kind == T_TYPE_ERROR ) {
        return t1;
    }
    return HOLLOW_T;
}

Typetree* check_program(Typetree *blockT) {
    if (blockT->kind == T_TYPE_ERROR) {
        return blockT;
    }
    return HOLLOW_T;
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

Typetree* check_arguments(char *id, ArgList *l2) {

    TypeNode *n1;
    TypeNode *n2;

    Typetree *subType;

    Typetree *t = lookupTable(current, id, 0)->type;

    if ( t->kind == T_FUNC ) {
        n1 = t->u.fun.dom->f;
        subType = t->u.fun.range;

    }  else if (t->kind == T_PROC ) {
        n1 = t->u.proc.dom->f;
        subType = HOLLOW_T;

    } else {
        fprintf(stderr, "FATAL: Fallo en el chequeo de tipos\n");
    }

    n2 = l2->f;

    if ( !compareTypeNodes(n1,n2) ) {
        fprintf(stderr, "Error: Tipos en la llamada de la funcion\n");
        return createType(T_TYPE_ERROR);
    } else {
        return subType;
    }

}

AST* set_node_type(AST* node, Typetree* type) {
    if(type->kind == T_TYPE_ERROR) {
        //destroy(node);
        //return newErrorNode(type);
        node->tag = N_ERROR;
        node->type = type;
        return node;
    } else {
        node->type = type;
        return node;
    }
}

void set_subrout_ast(char *id, AST *block) {
    if((temp = lookupTable(current, id, 0)) != NULL) {
        temp->ast = newFuncNode(temp, block);
    }
}

void set_last_dims_expr(AST* node, AST* new_node) {
    while(node->next != NULL)
        node = node->next;

    new_node->next = NULL;
    node->next = new_node;

}
