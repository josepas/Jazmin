

// No estoy claro si aqui va proc, no creo

typedef enum {INT, FLOAT, BOOL, CHAR, HOLLOW,
                FUNC, POINTER,  CONF, STRUCT, TYPE_ERROR} Kind;

typedef typetree Typetree;


struct typetree {
    Kind kind;
    int size;
    union {
        struct {
            int low;
            int high;
            Typetree *t;

        } a;
        struct {
            Typetree *t;
        } p;
        struct {
            Typetree *f;
            Typetree *s;
        } t;
        struct {
            Typetree *dom; // aqui creo que deberian entrar las tuplas
            Typetree *range; 
        } fun;
        struct {
            Typetree *campos
        } r;
    } u;
    typetree next;

} Typetree;