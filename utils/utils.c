#include "utils.h"

void* memory(size_t type) {

    void *ptr = malloc(type);
    if (ptr == NULL) {
        fprintf(stderr, "Fatal: Error en alocacion de memoria\n");
    }
    return ptr;
}

Offsetstack* createStack() {
    Offsetstack *new = (Offsetstack*) memory(sizeof(Offsetstack));
    new->offset = 0;
    new->prev = NULL;
    return new;
}

void push(Offsetstack *offs, int offset) {
    Offsetstack *new = (Offsetstack*) memory(sizeof(Offsetstack));
    new->offset = offs->offset;
    new->prev = offs->prev;
    offs->offset = offset;
    offs->prev = new;
}

int pop(Offsetstack *offs) {
    int offset = offs->offset;
    offs = offs->prev;
    return offset;
}