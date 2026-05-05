#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5

int main()
{
	srand((unsigned int)time(NULL));
	FILE* fp1 = fopen("01.txt", "w");
	fprintf(fp1, "%8d %8d\n", 0, 0);
	printf("%8d %8d\n", 0, 0);
	// int axis[2][MAX - 1];

	int i, x, y;
	for (i = 0; i < MAX - 1; i++)
	{
		x = rand() % 50;
		y = rand() % 50;

		fprintf(fp1, "%8d %8d\n", x, y);
		printf("%8d %8d\n", x, y);
	//	axis[0][i] = x;
	//	axis[1][i] = y;
	}

	/*
	int tempAxis[2][1];
	for (i = 0; i < MAX - 2; i++)
	{
		for (j = 0; j < MAX - 2 - i; j++)
		{
			int distance1 = axis[0][j] * axis[0][j] + axis[1][j] * axis[1][j];
			int distance2 = axis[0][j+1] * axis[0][j+1] + axis[1][j+1] * axis[1][j+1];
			if (distance1 > distance2)
			{
				tempAxis[0][0] = axis[0][j];
				axis[0][j] = axis[0][j + 1];
				axis[0][j + 1] = tempAxis[0][0];

				tempAxis[1][0] = axis[1][j];
				axis[1][j] = axis[1][j + 1];
				axis[1][j + 1] = tempAxis[1][0];
			}
		}
	}
	*/
	/*
	for (i = 0; i < MAX - 1; i++)
	{
		x = axis[0][i];
		y = axis[1][i];
		fprintf(fp1, "%8d %8d\n", x, y);
	}
	*/
	
	fclose(fp1);
	system("01.txt notepad.exe");
}