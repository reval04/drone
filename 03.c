#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
double distance(SLL* p);
void printDistance(double* d);
void freeLinkedList(SLLH* L);

int main()
{
	double dis[MAX - 1];
	FILE* fp3r = fopen("02.txt", "r");
	SLLH* nodeHead = createSingleLinkedList();

	int i;
	int x, y;
	for (i = 0; i < MAX; i++)
	{
		fscanf(fp3r, "%8d %8d", &x, &y);
		insertLastNode(nodeHead, x, y);
	}
	fclose(fp3r);

	SLL* temp = nodeHead->head;
	i = 0;
	for (i = 0; i < MAX - 1; i++)
	{
		dis[i] = distance(temp);
		temp = temp->next;
	}

	printDistance(dis);
	freeLinkedList(nodeHead);
	
	system("03.txt notepad.exe");
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

double distance(SLL* p)
{
	SLL* cur = p;
	SLL* nt = cur->next;

	double x = cur->x - nt->x;
	double y = cur->y - nt->y;

	return sqrt(x * x + y * y);
}

void printDistance(double* d)
{
	char* waypoint[] = { "AB", "BC", "CD", "DE" };
	char* menu[] = { "구간", "거리(m)", " k", "배터리 소모" };
	FILE* fp3w = fopen("03.txt", "w");

	fprintf(fp3w, "%10s %10s %10s %11s\n", *(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3));
	printf("%10s %10s %10s %11s\n", *(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3));

	int i;
	double k, batteryConsumed;
	double totaldis = 0, totalBatteryCon = 0;
	for (i = 0; i < MAX - 1; i++)
	{
		(*(d + i) > 100.0) ? (k = 1.7) : (k = 1.5);
		batteryConsumed = k * d[i];
		fprintf(fp3w, "%10s %10.1lf %10.1lf %11.1lf\n", *(waypoint + i), *(d + i), k, batteryConsumed);
		printf("%10s %10.1lf %10.1lf %11.1lf\n", *(waypoint + i), *(d + i), k, batteryConsumed);
		totaldis += *(d + i);
		totalBatteryCon += batteryConsumed;
	}

	char* t = "TOTAL";
	fprintf(fp3w, "%10s %10.1lf %10c %11.1lf\n", t, totaldis, '-', totalBatteryCon);
	printf("%10s %10.1lf %10c %10.1lf\n", t, totaldis, '-', totalBatteryCon);

	fclose(fp3w);
}
