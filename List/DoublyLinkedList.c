#include "DoublyLinkedList.h"

DLinkedList* newDoublyLinkList() {
    
    DLinkedList* new = (DLinkedList*)malloc(sizeof(DLinkedList));

    new->first = NULL;
    new->last = NULL;

    return new;
}

Node* newDLLNode(void* data) {

    Node* new = (Node*)malloc(sizeof(Node));
    
    new->index = -1;
    new->data = data;    
    new->prev = NULL;
    new->next = NULL;

    return new;
}

void reIndex(DLinkedList* list, int start) {

    for (Node* aux = list->first; aux->next != NULL; aux = aux->next) {
        aux->index = start;
        ++start;
    }

}

void add(DLinkedList* list, Node* who, int start) {

    if (list == NULL) {
        fprintf(stderr, "Error Fatal agregando a la lista de quads\n");
    }

    if (list->first == NULL) {
        list->first = who;
        list->last = who;
    } else {
        // Agregar al inicio
        if (start) {
            who->next = list->first;
            list->first->prev = who;
            list->first = who;
            reIndex(list, who->next->index);            
        // Agregar al final
        } else {
            who->index = list->last->index + 1;
            list->last->next = who;
            who->prev = list->last;
            list->last = who;
        }
    }

}


// Chuequear con calma
void removeN(DLinkedList* list, Node* who) {
    
    if (list == NULL) {
        fprintf(stderr, "Error Fatal eliminando a la lista de quads\n");
        return;
    }    

    if (list->first == NULL) {
        fprintf(stderr, "Error eliminando en lista de quads vacia\n");        
        return;
    }

    // Un solo nodo
    if (list->first == list->last) {
        list->first = NULL;
        list->last = NULL;
        free(who);
        return;
    }

    // Primero
    if (who == list->first) {
        list->first = list->first->next;
    }

    // Ultimo
    if (who == list->last) {
        list->last = list->last->prev;
    } else {
        who->prev->next = who->next;
        who->next->prev = who->prev;
    }

    free(who);

}


void destroyHelper(Node* who) {
    
    if (who != NULL) 
        return;

    destroyHelper(who->next);
    // Aqui quizas hay que mandar a liberar el quad
    // pero creo que me tendrias que pasar la funcion 
    // para liberarlo. (no muy claro)
    free(who);
}

void destroyDLList(DLinkedList* list) {
    
    destroyHelper(list->first);
    free(list);
}



