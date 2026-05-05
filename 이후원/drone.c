#include "drone.h"

SLLH* createSingleLinkedList()
{
	SLLH* h;
	h = (SLLH*)malloc(sizeof(SLLH));
	h->head = NULL;
	return h;
}

void insertLastNode(SLLH* L, int x, int y)
{
	int count = 1;
	SLL* newNode;
	SLL* temp;
	newNode = (SLL*)malloc(sizeof(SLL));
	newNode->x = x;
	newNode->y = y;
	newNode->next = NULL;
	if (L->head == NULL) {
		newNode->name = 'A';
		L->head = newNode;
		return;
	}

	temp = L->head;
	while (temp->next != NULL)
	{
		temp = temp->next;
		count++;
	}
	temp->next = newNode;
	newNode->name = 'A' + count;
	count++;
}

void freeLinkedList(SLLH* L)
{
	SLL* p;
	while (L->head != NULL) {
		p = L->head;
		L->head = L->head->next;
		free(p);
		p = NULL;
	}
}
