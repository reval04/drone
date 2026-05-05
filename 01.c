#include "drone.h"

#define MAX 5

void makeWayPoint()
{
	srand((unsigned int)time(NULL));
	FILE* fp1 = fopen("01.txt", "w");
	fprintf(fp1, "%8d %8d\n", 0, 0);
	printf("%8d %8d\n", 0, 0);

	int i, x, y;
	for (i = 0; i < MAX - 1; i++)
	{
		x = rand() % 50;
		y = rand() % 50;

		fprintf(fp1, "%8d %8d\n", x, y);
		printf("%8d %8d\n", x, y);
	}

	fclose(fp1);
	printf("===================================================\n");
	system("notepad.exe 01.txt");
}
