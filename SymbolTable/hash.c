#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct _entry {
    char *string;
    int row;
    int column;
    struct _list *next;
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

uint32_t hash(char *key, size_t len) {

    uint32_t hash;

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
    for(i=0; i<size; i++) nHash->table[i] = NULL;

    nHash->size = size;

    return nHash;

}


int main(int argc, char const *argv[]) {
	return 0;
}