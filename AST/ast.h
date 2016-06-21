typedef enum {N_IF, N_WHILE, N_BIN_OP, N_UN_OP, N_INT, N_CHAR, N_BOOL, N_VAR, N_ASGN...} NodeTag;


typedef struct _ast {
    NodeTag tag;

    char *id;
    char *operation;

    struct _ast *first;
    struct _ast *last;
    struct _ast *next;

    union {
        struct _entry *sym;
        int i;
        int b;
        float f;
        char c;
    } u;

} AST;



