#include "drone.h"


void makeDistancePerBatteryTable()
{
	double dis[MAX - 1];
	FILE* fp3r = fopen("02.txt", "r");
	SLLH* nodeHead = createSingleLinkedList();

	int i;
	int x, y;
	char name;
	for (i = 0; i < MAX; i++)
	{
		fscanf(fp3r, " %c %8d %8d",&name, &x, &y);
		insertLastNode(nodeHead, x, y, name);
	}
	fclose(fp3r);

	SLL* temp = nodeHead->head;
	i = 0;
	for (i = 0; i < MAX - 1; i++)
	{
		dis[i] = distance(temp);
		temp = temp->next;
	}

	temp = nodeHead->head;
	printDistance(dis, temp);
	freeLinkedList(nodeHead);
	printf("===================================================\n");

	system("notepad.exe 03.txt");
}
