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
    Entry **table;
} Hash;


void* memory(size_t);

unsigned long hash(char*);

Hash* createHash();

Entry* lookupHash(Hash*, char*);

void insertHash(Hash*, char*, int, int);

void dumpHash(Hash*, int);

void destroyHash(Hash*);
