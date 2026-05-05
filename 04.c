#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX 5
#define MAX_B 250

typedef struct singleLinkedList
{
	char point[11];
	double d;
	double k;
	double batteryCon;
	struct singleLinkedList* next;
}SLL;

SLL* top = NULL;

void push(char* wp, double item, double k, double bc);
SLL* pop();
char* navigate(SLL* t);
int isStackEmpty();

int main()
{
	FILE* fp4 = fopen("03.txt", "r");
	FILE* fp4w = fopen("04.txt", "w");
	if (fp4 == NULL || fp4w == NULL)
	{
		printf("파일이 제대로 열리지 않음\n");
		return -1;
	}

	double distance, k, batteryConsumed;
	char p[11];
	char* na;
	int i;
	double totalDis = 0, totalBatteryCon = 0;

	
	char* menu[] = { "구간", "거리(m)", " k", "배터리 소모", "누적 소모량", "상태", "이벤트", "안정회귀 경로"};
	fprintf(fp4w, "%10s %10s %10s %11s %11s %10s %10s %11s\n", 
		*(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3), *(menu + 4), *(menu + 5), *(menu + 6), *(menu + 7));
	printf("%10s %10s %10s %11s %11s %10s %10s %11s\n",
		*(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3), *(menu + 4), *(menu + 5), *(menu + 6), *(menu + 7));

	
	char dummy[256];
	fgets(dummy, sizeof(dummy), fp4);

	for (i = 0; i < MAX - 1; i++)
	{
		fscanf(fp4, "%10s %10lf %10lf %11lf", p, &distance, &k, &batteryConsumed);
		if (totalBatteryCon + batteryConsumed < MAX_B / 2.0)
		{
			push(p, distance, k, batteryConsumed);
			totalBatteryCon += batteryConsumed;
			fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			fprintf(fp4w, "%10s %10c %10c\n", "NORMAL", '-', '-'); fflush(fp4w);
			printf("%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			printf("%10s %10c %10c\n", "NORMAL", '-', '-');
		}
		else
		{
			totalBatteryCon += batteryConsumed;
			na = navigate(top);
			fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			fprintf(fp4w, "%10s %10s %10s\n", "FAILURE", "미션실패", na); fflush(fp4w);
			printf("%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			printf("%10s %10s %10s\n", "FAILURE", "미션실패", na);
			free(na);
			break;
		}
	}

	if (top != NULL)
	{
		SLL* below = top->next;
		SLL* getPop;
		char revPoint[12];
		char ret[20];

		while (!isStackEmpty())
		{
			getPop = pop();
			totalBatteryCon += getPop->batteryCon;
			
			sprintf(revPoint, "%c%c", getPop->point[1], getPop->point[0]);
			fprintf(fp4w, "%10s %10c %10c %10c %10.1lf", revPoint, '-', '-', '-', totalBatteryCon);
			printf("%10s %10c %10c %10c %10.1lf", revPoint, '-', '-', '-', totalBatteryCon);

			sprintf(ret, "%c→%c", getPop->point[1], getPop->point[0]);

			fprintf(fp4w, "%10s %10s %10s\n", "RECOVERY", "역추적", ret);
			printf("%10s %10s %10s\n", "RECOVERY", "역추적", ret);

			if (below == NULL)
			{
				break;
			}
			below = below->next;
		}
	}
	while (!isStackEmpty())
	{
		free(pop());
	}

	fclose(fp4);
	fclose(fp4w);
	system("notepad.exe 04.txt");

	return 0;
}

int isStackEmpty()
{
	if (top == NULL) return 1;
	else return 0;
}

void push(char* wp, double item, double k, double bc)
{
	SLL* temp = (SLL*)malloc(sizeof(SLL));

	strcpy(temp->point, wp);
	temp->d = item;
	temp->k = k;
	temp->batteryCon = bc;
	temp->next = top;
	top = temp;
}

SLL* pop()
{
	SLL* temp = top;

	if (isStackEmpty())
	{
		printf("스택이 비었습니다. \n");
		return 0;
	}
	else
	{
		top = temp->next;
		return temp;
	}
}

char* navigate(SLL* t)
{
	char* nav = (char *)malloc(20 * MAX * sizeof(char));
	nav[0] = '\0';

	SLL* curr = t;
	while (curr != NULL)
	{
		strcat(nav, curr->point);
		if (curr->next != NULL)
		{
			strcat(nav, "→");
		}
		curr = curr->next;
	}
	
	return nav;
}
