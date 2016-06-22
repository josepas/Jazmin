#include "utils.h"

void* memory(size_t type) {

    void *ptr = malloc(type);
    if (ptr == NULL) {
        fprintf(stderr, "Fatal: Error en alocacion de memoria\n");
    }
    return ptr;
}

void printStack(Offsetstack *offs) {
    while(offs != NULL) {
        printf("%d -> ", offs->offset);
        offs = offs->prev;
    }
    printf("\n");
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
    // offs->prev NUNCA sera NULL
    Offsetstack *temp = offs->prev;
    offs->offset = offs->prev->offset;
    offs->prev = offs->prev->prev;
    free(temp);
    return offset;
}
