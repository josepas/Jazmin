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


void* memory(size_t);

unsigned long hash(char*, int);

Hash* createHash(int);

Entry* lookupHash(Hash*, char*);


void insertHash(Hash*, char*, int, int);


void dumpHash(Hash*, int);

void destroyHash(Hash*);


// int main(int argc, char const *argv[]) {
//     Hash* h = createHash(17);
//  insert(h, "a", 1, 2);
//     insert(h, "cola", 1, 2);
//     insert(h, "gusano", 1, 2);
//     insert(h, "hashTabl", 1, 2);
//     insert(h, "Hashy Potter", 1, 2);

//     dump(h);
//     destroy(h);
//     return 0;
// }