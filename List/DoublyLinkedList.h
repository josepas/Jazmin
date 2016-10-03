#ifndef DLINKEDLIST 
#define DLINKEDLIST

#include <stdio.h>
#include <stdlib.h>


typedef struct _node {

    int index;

    void *data;

    struct _node *prev;
    struct _node *next;
} Node;

typedef struct _dlinkedlist {
    
    Node* first;
    Node* last;

} DLinkedList;


DLinkedList* newDoublyLinkList();
Node* newDLLNode(void*);
void addDLL(DLinkedList*, Node*, int);
void removeN(DLinkedList* list, Node* who);
void destroyDLList(DLinkedList*);

#endif