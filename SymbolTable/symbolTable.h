typedef struct entrytag {
	char* table;
	int row;
	int column;
} Entry;


typedef struct _symtable {
	struct sttag *father;
	struct sttag *fchild, *lchild;
	struct sttag *next;	
	Entry table;
} Symtable;



void enterScope(Symtable*);
void exitScope();
int lookup(Symtable*);
void destroy(Symtable*);

