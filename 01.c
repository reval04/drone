#include "drone.h"

#define MAX 5

void makeWayPoint() // 드론 지점 생성 함수
{
	srand((unsigned int)time(NULL)); // 무작위 난수값을 생성하기 위한 시드값 설정
	FILE* fp1 = fopen("01.txt", "w"); // 01.txt 쓰기 모드로 열기
	fprintf(fp1, "%8d %8d\n", 0, 0); // A 좌표를 (0, 0)으로 설정후 파일에 쓰기
	printf("%8d %8d\n", 0, 0);

	int i, x, y;
	for (i = 0; i < MAX - 1; i++)
	{
		x = rand() % 50; // 0부터 49까지의 난수 발생
		y = rand() % 50; // 0부터 49까지의 난수 발생

		fprintf(fp1, "%8d %8d\n", x, y);
		printf("%8d %8d\n", x, y);
	}

	fclose(fp1);
	printf("===================================================\n");
	system("notepad.exe 01.txt");
}
