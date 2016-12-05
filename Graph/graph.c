#include "graph.h"

extern unsigned int l_num;

void newGraph(Node* code) {
    Graph* g = (Graph*)malloc(sizeof(Graph));

    g->code = code;
    g->next = NULL;
    g->jump = NULL;
}

DLinkedList* makeBlocks(Graph *g) {
    DLinkedList *list = newDoublyLinkedList();
    addDLL(list, newDLLNode(g), 0);

    Graph *g_temp = g;
    Graph* graph_list[l_num];
    memset(graph_list, 0, sizeof(graph_list));

    Node *ins = g_temp->code;
    Quad *q;

    for(ins = g_temp->code; ins != NULL; ins=ins->next) {
        q = ((Quad*)ins->data)
        switch(q->op) {
            case GOTO:
                g_temp = newGraph(ins->next);
                ins->next->prev = NULL;
                ins->next = NULL;
                addDLL(list, newDLLNode(g_temp), 0);
                break;
            case IF_GOTO:
            case IFN_GOTO:
            case OP_EQUAL:
            case OP_UNEQUAL:
            case OP_LT:
            case OP_LTOE:
            case OP_GT:
            case OP_GTOE:
                g_temp->next = newGraph(ins->next);
                ins->next->prev = NULL;
                ins->next = NULL;
                g_temp = g_temp->next;
                addDLL(list, newDLLNode(g_temp), 0);
                break;
            case OP_LABEL:
                if(g_temp->code->first == ins) {
                    continue;
                else {
                    g_temp = newGraph(ins);
                    graph_list[q->arg1->u.l] = g_temp;
                    ins->prev->next = NULL;
                    ins->prev = NULL;
                    addDLL(list, newDLLNode(g_temp), 0);
                }
                break;
        }
    }

    // Falta conectar jumps
    // g_temp = g;
    // ins = g_temp->code->first;
    // for(; g_temp != NULL; g_temp=g_temp->next) {
    // }

    return list;
}