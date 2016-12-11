#include "symbolTable.h"


unsigned long hash(char *key) {

    unsigned long hash = 0;

    for(; *key != '\0'; key++) {
        hash += *key;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % HASH_SIZE;
}


Symtable* createTable(Symtable *father) {

	Symtable* newTable;

	newTable = (Symtable*) memory(sizeof(Symtable));
    newTable->level = 0;
	newTable->size = 0;
	newTable->father = father;
	newTable->fchild = NULL;
	newTable->lchild = NULL;
	newTable->next  = NULL;
	newTable->table = memory( (sizeof(Entry *) * HASH_SIZE) );

	int i;
    for(i=0; i<HASH_SIZE; i++) {
        newTable->table[i] = NULL;
    }

	return newTable;
}


Symtable* enterScope(Symtable *current) {

	Symtable *nTable = createTable(current);

	if (current == NULL) {
		nTable->level = 0;
	} else if ( current->fchild == NULL ) {
		nTable->level = current->level + 1;
		current->fchild = nTable;
		current->lchild = nTable;
	} else {
		nTable->level = current->level + 1;
		current->lchild->next = nTable;
		current->lchild = nTable;
	}

	return nTable;
}

Symtable* enterScopeR(Symtable* current, Symtable* helper) {

	Symtable *nTable;

	if (helper == NULL) {
		nTable = createTable(current);
		return nTable;
	} else {
		nTable = createTable(helper);
	}

	if ( helper->fchild == NULL ) {
		helper->fchild = nTable;
		helper->lchild = nTable;
	} else {
		helper->lchild->next = nTable;
		helper->lchild = nTable;
	}

	return nTable;

}


Symtable* exitScope(Symtable *current) {
	return current->father;
}


void dumpTable(Symtable *printer) {
	printf("%*sScope %d:%d\n", printer->level*3, "", printer->level, printer->size);

    int i;
    Entry *aux;
    for(i=0; i<HASH_SIZE; i++) {
        aux = printer->table[i];
        while(aux!=NULL) {
            printf("%*s-%s %d:%d ", printer->level*3, "", aux->string, aux->line, aux->column);
            dumpType(aux->type);
            printf(" size:%d  offset:%d\n", aux->size, aux->offset);

            // if(aux->clase == C_RECORD) {
            //     printf("STRUCTURA!!\n");
            //     dumpTable(aux->type->u.r.fields);
            //     printf("FIN --- STRUCTURA!!\n");
            // }

            aux = aux->next;
        }
    }

	printf("========================================\n");

	if (printer->fchild != NULL) {
		dumpTable(printer->fchild);
	}
	if (printer->next != NULL) {
		dumpTable(printer->next);
	}

}


void destroyTable(Symtable *who) {

	if ( who->fchild != NULL ) {
		destroyTable(who->fchild);
	}
	if ( who->next != NULL ) {
		destroyTable(who->next);
	}

    int i;
    Entry *list, *temp;
    for(i=0; i<HASH_SIZE; i++) {
        list = who->table[i];
        while(list!=NULL) {
            temp = list;
            list = list->next;
            free(temp->string);
            free(temp);
        }
    }

    free(who->table);

	free(who);
}


void insertTable(Symtable *current, char *str, int line, int column, Class clase, Typetree *type, int size, int offset) {

	if(lookupTable(current, str, 1) == NULL) {
	    Entry *newEntry;
		unsigned long h = hash(str);

	    newEntry = (Entry*) memory(sizeof(Entry));

	    newEntry->string = strdup(str);
	    newEntry->line = line;
	    newEntry->column = column;
        newEntry->clase = clase;
        newEntry->type = type;
        newEntry->size = size;
        newEntry->offset = offset;
	    newEntry->next = current->table[h];
        newEntry->scope = current->level > 1 ? LOCAL : GLOBAL;
        newEntry->ast = NULL;

        current->table[h] = newEntry;
        current->size = offset+size;

	}
	else {
		fprintf(stderr, "Error:%d:%d: Redeclaración %s\n", line, column, str);
	}
}


Entry* lookupTable(Symtable* current, char* key, int local) {
	if(current == NULL) {
		return NULL;
	}

	unsigned long h = hash(key);
    Entry* aux;

    for(aux = current->table[h]; aux != NULL; aux = aux->next) {
        if (strcmp(key, aux->string) == 0) return aux;
    }

    // local == 0 significa busqueda exclusivamente local
	if(local == 0) {
		return lookupTable(current->father, key, local);
	}

	return aux;
}

int getMaxSize(Symtable* current) {
    int i;
    int max = 0;
    Entry *aux;
    for(i=0; i<HASH_SIZE; i++) {
        aux = current->table[i];
        while(aux!=NULL) {
            if(max < aux->size)
                max = aux->size;
            aux = aux->next;
        }
    }
    return max;
}

void arreglarAux(Symtable* tabla, int inc) {
    int i;
    Entry *aux;
    for(i=0; i<HASH_SIZE; i++) {
        aux = tabla->table[i];
        while(aux!=NULL) {
            // if(aux->type->kind != T_CONF && aux->type->kind != T_STRUCT) {
                aux->offset += inc;
                aux->offset *= -1;
            // }
            aux = aux->next;
        }
    }

    if (tabla->fchild != NULL) {
        arreglarAux(tabla->fchild, inc + tabla->size);
    }
    if (tabla->next != NULL) {
        arreglarAux(tabla->next, inc);
    }
}

void arreglarTabla(Symtable* tabla) {
    // Arreglar tablas de subrutinas
    tabla = tabla->fchild;  // 1
    Symtable *aux = tabla->fchild;     // 2
    while(aux != tabla->lchild) {
        arreglarAux(aux->fchild, 0);
        aux = aux->next;
    }

    // Arreglar tablas del program
    arreglarAux(tabla->lchild, 0);
}