#ifndef DEF_GRAPH
#define DEF_GRAPH
#include "List/DoublyLinkedList.h"


typedef struct _graph {

    Node *code;

    struct _graph next;
    struct _graph jump;


} Graph;

void newGraph(Node*);

void makeBlocks(Graph*);

#endif