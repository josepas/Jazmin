#ifndef DEF_GRAPH
#define DEF_GRAPH
#include "List/DoublyLinkedList.h"


typedef struct _graph {

    DLinkedList *code;

    struct _graph next;
    struct _graph jump;


} Graph;

void newGraph(DLinkedList*);

void newGraph(DLinkedList* code) {
    Graph* g = (Graph*)malloc(sizeof(Graph));

    g->code = code;
    g->next = NULL;
    g->jump = NULL;

}


#endif