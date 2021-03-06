#ifndef TYPETREE
#define TYPETREE
#include "../SymbolTable/symbolTable.h"
#include "../utils/utils.h"

typedef enum {T_INT, T_FLOAT, T_BOOL, T_CHAR, T_HOLLOW, //0..4
                T_FUNC, T_PROC, T_POINTER, T_ARRAY, T_CONF, T_STRUCT,   //5..10
                T_TYPE, T_TYPE_ERROR} Kind; //11..12

typedef struct _typeNode {
    struct _typetree *t;
    struct _typeNode *next;
} TypeNode;

typedef struct _argList {
    TypeNode *f;
    TypeNode *l;
} ArgList;


typedef struct _typetree {
    Kind kind;
    int size;
    union {
        struct {
            int low;
            int high;
            struct _typetree *t;

        } a;
        struct {
            struct _typetree *t;
        } p;
        struct {
            ArgList *dom;
            struct _typetree *range;
        } fun;
        struct {
            ArgList *dom;
            // *range puede que convenga fucionar con func
        } proc;
        struct {
            char *name;
            struct _symtable *fields;
        } r;
    } u;

} Typetree;



ArgList* newArgList();
ArgList* add(ArgList*, struct _typetree*);
int compareTypeNodes(TypeNode*, TypeNode*);

Typetree* createType(Kind);
Typetree* createArray(int, Typetree*);
Typetree* createStruct(char *name);
Typetree* createConf(char *name);
Typetree* createProc(ArgList*);
Typetree* createFunc(ArgList *list, Typetree *range);
void dumpType(Typetree*);

#endif