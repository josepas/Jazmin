#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct _entry {
    char *string;
    int row;
    int column;
    struct _entry *next;
} Entry;


typedef struct _hash {
    int size;       
    Entry **table; 
} Hash;


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

Entry* lookup(Hash *where, char* key) {
    
    unsigned long h = hash(key, where->size);
    Entry* aux;

    for(aux = where->table[h]; aux != NULL; aux = aux->next) {
        if (strcmp(key, aux->string) == 0) return aux;
    }
    return NULL;

}


void insert(Hash *where, char *key, int row, int column) {

    Entry *newEntry;
    Entry *aux;
    unsigned long h = hash(key, where->size);

    newEntry = (Entry*) memory(sizeof(Entry));

    aux = lookup(where, key);
    if (aux != NULL) {
        fprintf(stderr, "ya existe papah\n");
    }

    newEntry->string = strdup(key);
    newEntry->next = where->table[h];
    where->table[h] = newEntry;

}

void destroy(Hash *hashtable)
{
    int i;
    Entry *list, *temp;

    if (hashtable==NULL) return;

    for(i=0; i<hashtable->size; i++) {
        list = hashtable->table[i];
        while(list!=NULL) {
            temp = list;
            list = list->next;
            free(temp->string);
            free(temp);
        }
    }

    free(hashtable->table);
    free(hashtable);
}


int main(int argc, char const *argv[]) {
	return 0;
}