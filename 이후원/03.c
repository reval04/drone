#include "drone.h"

#define MAX 5

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
		fscanf(fp3r, " %c %8d %8d", &name, &x, &y);
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

double distance(SLL* p)
{
	SLL* cur = p;
	SLL* nt = cur->next;

	double x = cur->x - nt->x;
	double y = cur->y - nt->y;

	return sqrt(x * x + y * y);
}

void printDistance(double* d, SLL* t)
{
	char wayPoint[3];
	char* menu[] = { "구간", "거리(m)", " k", "배터리 소모" };
	FILE* fp3w = fopen("03.txt", "w");

	fprintf(fp3w, "%10s %10s %10s %11s\n", *(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3));
	printf("%10s %10s %10s %11s\n", *(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3));

	SLL* n = t->next;
	int i;
	double k, batteryConsumed;
	double totaldis = 0, totalBatteryCon = 0;
	for (i = 0; i < MAX - 1; i++)
	{
		wayPoint[0] = t->name;
		wayPoint[1] = n->name;
		wayPoint[2] = '\0';
		(*(d + i) > 100.0) ? (k = 1.7) : (k = 1.5);
		batteryConsumed = k * d[i];
		fprintf(fp3w, "%10s %10.1lf %10.1lf %11.1lf\n", wayPoint, *(d + i), k, batteryConsumed);
		printf("%10s %10.1lf %10.1lf %11.1lf\n", wayPoint, *(d + i), k, batteryConsumed);
		totaldis += *(d + i);
		totalBatteryCon += batteryConsumed;
		if (n->next != NULL)
		{
			t = t->next;
			n = n->next;
		}
	}

	char* tot = "TOTAL";
	fprintf(fp3w, "%10s %10.1lf %10c %11.1lf\n", tot, totaldis, '-', totalBatteryCon);
	printf("%10s %10.1lf %10c %10.1lf\n", tot, totaldis, '-', totalBatteryCon);

	fclose(fp3w);
}