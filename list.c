#include <stdlib.h>


struct List {
	struct Node *head;
	struct Node *last;
}

struct Node {
	void *data;
	struct Node* next;
}

List* createList() {
	
	List newList; 
	list = (List*) malloc(sizeof(List));

	newList->head = NULL;
	newList->last = NULL;

	return newList;
}

void agregar(List *who, void* what) {

	Node *node;
	node = (Node*) malloc(sizeof(Node));

	node->next = NULL

	node->data = what;
 
	if (who->head == NULL) {
		who.head = node;
		who.last = node;

	} else {
		who->last->next = node;
		who->last = node;
	}  

}




