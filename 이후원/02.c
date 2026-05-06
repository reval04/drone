#include "drone.h"

#define MAX 5

void arrangeWayPoint()
{
	FILE* fp2 = fopen("01.txt", "r");
	SLLH* head = createSingleLinkedList();
	SLL* temp;

	int i, x, y;
	for (i = 0; i < MAX; i++)
	{
		fscanf(fp2, "%8d %8d", &x, &y);
		insertLastNode(head, x, y, NULL);
	}
	fclose(fp2);
	
	bubbleSort(head);
	pinPointing(head);
	FILE * fp2w = fopen("02.txt", "w");
	temp = head->head;

	while (temp != NULL)
	{
		fprintf(fp2w, "%8c %8d %8d\n",temp->name, temp->x, temp->y);
		printf("%8c %8d %8d\n", temp->name, temp->x, temp->y);
		temp = temp->next;
	}
	fclose(fp2w);
	freeLinkedList(head);
	printf("===================================================\n");
	system("notepad.exe 02.txt");
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
		current = L->head;
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

void pinPointing(SLLH* L)
{
	SLL* temp = L->head;
	int i = 0;

	while (temp != NULL)
	{
		temp->name = 'A' + i;
		i++;
		temp = temp->next;
	}
}
