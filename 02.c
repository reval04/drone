#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5

typedef struct singleLinkedList
{
	int x;
	int y;
	struct singleLinkedList* next;
}SLL;

typedef struct singleLinkedListHead
{
	SLL* head;
}SLLH;

SLLH* createSingleLinkedList();
void insertLastNode(SLLH* L, int x, int y);
void bubbleSort(SLLH* L);
void freeLinkedList(SLLH* L);

int main()
{
	FILE* fp2 = fopen("01.txt", "r");
	SLLH* head = createSingleLinkedList();
	SLL* temp;

	int i, x, y;
	for (i = 0; i < MAX; i++)
	{
		fscanf(fp2, "%8d %8d", &x, &y);
		insertLastNode(head, x, y);
	}
	fclose(fp2);
	
	bubbleSort(head);
	FILE * fp2w = fopen("02.txt", "w");
	temp = head->head;

	while (temp != NULL)
	{
		fprintf(fp2w, "%8d %8d\n", temp->x, temp->y);
		printf("%8d %8d\n", temp->x, temp->y);
		temp = temp->next;
	}
	fclose(fp2w);
	freeLinkedList(head);
	system("02.txt notepad.exe");
}

SLLH* createSingleLinkedList()
{
	SLLH* h;
	h = (SLLH*)malloc(sizeof(SLLH));
	h->head = NULL;
	return h;
}

void insertLastNode(SLLH* L, int x, int y)
{
	SLL* newNode;
	SLL* temp;
	newNode = (SLL*)malloc(sizeof(SLL));
	newNode->x = x;
	newNode->y = y;
	newNode->next = NULL;
	if (L->head == NULL) {
		L->head = newNode;
		return;
	}

	temp = L->head;
	while (temp->next != NULL) temp = temp->next;
	temp->next = newNode;
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

void bubbleSort(SLLH* L)
{
	if (L == NULL || L->head == NULL)
	{
		return;
	}

	SLL* current = L->head;
	int i, j;
	int distance1, distance2;
	for (i = 0; i < MAX - 1; i++)
	{
		for (j = 0; j < MAX - 1 - i; j++)
		{
			if (current != NULL && current->next != NULL)
			{
				distance1 = current->x * current->x + current->y * current->y;
				distance2 = current->next->x * current->next->x + current->next->y * current->next->y;

				if (distance1 > distance2)
				{
					int xTemp, yTemp;
					xTemp = current->x; yTemp = current->y;
					current->x = current->next->x; current->y = current->next->y;
					current->next->x = xTemp; current->next->y = yTemp;
				}
				current = current->next;
			}
		}
	}
}
