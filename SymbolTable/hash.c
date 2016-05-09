#include "hash.h"


void* memory(size_t type) {

    void *ptr = malloc(type);
    if (ptr == NULL) {
        fprintf(stderr, "Fatal: Error en alocacion de memoria\n");
    }
    return ptr;
}


unsigned long hash(char *key, int len) {

    unsigned long hash;

    for(; *key != '\0'; key++) {
        hash += *key;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % len;
}


Hash* createHash(int size) {

    Hash* nHash = (Hash*) memory(sizeof(Hash));
    nHash->table = (Entry**) memory( (sizeof(Entry *) * size) );

    int i;
    for(i=0; i<size; i++) {
        nHash->table[i] = NULL;
    }

    nHash->size = size;

    return nHash;

}


Entry* lookupHash(Hash *where, char* key) {

    unsigned long h = hash(key, where->size);
    Entry* aux;

    for(aux = where->table[h]; aux != NULL; aux = aux->next) {
        if (strcmp(key, aux->string) == 0) return aux;
    }
    return NULL;

}


void insertHash(Hash *where, char *key, int row, int column) {

    Entry *newEntry;
    Entry *aux;
    unsigned long h = hash(key, where->size);

    newEntry = (Entry*) memory(sizeof(Entry));

    aux = (Entry*) lookupHash(where, key);
    if (aux != NULL) {
        fprintf(stderr, "ya existe papah\n");
    }

    newEntry->string = strdup(key);
    newEntry->row = row;
    newEntry->column = column;
    newEntry->next = where->table[h];
    where->table[h] = newEntry;

}


void dumpHash(Hash* hashTable, int level) {
    int i;
    Entry *aux;

    if (hashTable==NULL) printf("Vacia\n");

    for(i=0; i<hashTable->size; i++) {
        aux = hashTable->table[i];
        while(aux!=NULL) {
            printf("%*s-%s %d:%d\n", level*3, " ", aux->string, aux->row, aux->column);
            aux = aux->next;
        }
    }

}


void destroyHash(Hash *hashTable) {
    int i;
    Entry *list, *temp;

    if (hashTable==NULL) return;

    for(i=0; i<hashTable->size; i++) {
        list = hashTable->table[i];
        while(list!=NULL) {
            temp = list;
            list = list->next;
            free(temp->string);
            free(temp);
        }
    }

    free(hashTable->table);
    free(hashTable);
}
