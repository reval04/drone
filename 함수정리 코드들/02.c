#include "drone.h"

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
	FILE* fp2w = fopen("02.txt", "w");
	temp = head->head;

	while (temp != NULL)
	{
		fprintf(fp2w, "%8c %8d %8d\n", temp->name, temp->x, temp->y);
		printf("%8c %8d %8d\n", temp->name, temp->x, temp->y);
		temp = temp->next;
	}
	fclose(fp2w);
	freeLinkedList(head);
	printf("===================================================\n");
	system("notepad.exe 02.txt");
}
