

typedef enum {N_IF, N_WHILE, N_BIN_OP, N_UN_OP ...} NodeTag;


typedef struct _ast {
    NodeTag tag;

    // no se si necesito padre
    char *operation;

    struct _ast *first;
    struct _ast *last;
    struct _ast *next;

    // Symtable *table;

} AST;



