typedef struct entrytag {
	char* table;
	int row;
	int column;
} Entry;


typedef struct _symtable {
	struct _symtable *father;
	struct _symtable *fchild, *lchild;
	struct _symtable *next;	
	Entry table;
} Symtable;



void enterScope(Symtable*);
void exitScope();
int lookup(Symtable*);
void destroy(Symtable*);

