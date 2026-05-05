#include "drone.h"

#define MAX 5
#define MAX_B 250

SLL4* top = NULL;

int main()
{
	makeWayPoint();
	arrangeWayPoint();
	makeDistancePerBatteryTable();
	FILE* fp4 = fopen("03.txt", "r");
	FILE* fp4w = fopen("04.txt", "w");
	if (fp4 == NULL || fp4w == NULL) // 03.txt와 04.txt이 모두 정상적으로 열렸는지 확인
	{
		printf("파일이 제대로 열리지 않음\n");
		return -1;
	}

	double distance, k, batteryConsumed;
	char p[11]; // 구간
	char* na;
	int i;
	double totalDis = 0, totalBatteryCon = 0;


	char* menu[] = { "구간", "거리(m)", " k", "배터리 소모", "누적 소모량", "상태", "이벤트", "안정회귀 경로" };
	fprintf(fp4w, "%10s %10s %10s %11s %11s %10s %10s %11s\n",
		*(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3), *(menu + 4), *(menu + 5), *(menu + 6), *(menu + 7)); // 상단 메뉴 출력
	printf("%10s %10s %10s %11s %11s %10s %10s %11s\n",
		*(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3), *(menu + 4), *(menu + 5), *(menu + 6), *(menu + 7)); // 상단 메뉴 출력


	char dummy[256];
	fgets(dummy, sizeof(dummy), fp4); // 첫줄은 메뉴부분이므로 읽어서 버림

	for (i = 0; i < MAX - 1; i++)
	{
		fscanf(fp4, "%10s %10lf %10lf %11lf", p, &distance, &k, &batteryConsumed); // 파일에서 구간, 거리, 소모계수, 배터리 소모량 받아오기
		if (totalBatteryCon + batteryConsumed < MAX_B / 2.0) // 누적 배터리 소모량이 드론의 배터리 용량의 절반보다 작은경우(임무수행 가능상태)
		{
			push(p, distance, k, batteryConsumed); // 스택에 파일에서 받아온 정보 저장
			totalBatteryCon += batteryConsumed; // 누적 배터리 소모량 계산
			fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			fprintf(fp4w, "%10s %10c %10c\n", "NORMAL", '-', '-'); fflush(fp4w); // 버퍼에 저장된 내용 fp4w파일에 기록
			printf("%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			printf("%10s %10c %10c\n", "NORMAL", '-', '-');
		}
		else // 임무수행 불가능상태
		{
			totalBatteryCon += batteryConsumed; // 누적 배터리 소모량 계산
			na = navigate(top); // 스택에 저장되었던 정보들을 이용하여 안전회귀	경로 생성
			fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			fprintf(fp4w, "%10s %10s %10s\n", "FAILURE", "미션실패", na); fflush(fp4w);
			printf("%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, distance, k, batteryConsumed, totalBatteryCon);
			printf("%10s %10s %10s\n", "FAILURE", "미션실패", na);
			free(na); // nav 동적메모리 반환
			break;
		}
	}

	if (top != NULL) // 스택이 공백상태인지 확인
	{
		SLL4* below = top->next;
		SLL4* getPop;
		char revPoint[12];
		char ret[20];

		while (!isStackEmpty()) // 스택이 공백상태가 아닐때까지 반복
		{
			getPop = pop();
			totalBatteryCon += getPop->batteryCon;

			sprintf(revPoint, "%c%c", getPop->point[1], getPop->point[0]); // 구간을 revpoint 배열에 저장
			fprintf(fp4w, "%10s %10c %10c %10c %10.1lf", revPoint, '-', '-', '-', totalBatteryCon);
			printf("%10s %10c %10c %10c %10.1lf", revPoint, '-', '-', '-', totalBatteryCon);

			sprintf(ret, "%c→%c", getPop->point[1], getPop->point[0]);
			fprintf(fp4w, "%10s %10s %10s\n", "RECOVERY", "역추적", ret);
			printf("%10s %10s %10s\n", "RECOVERY", "역추적", ret);

			if (below == NULL) // 스택이 끝까지 내려왔다면 종료
			{
				break;
			}
			below = below->next;
		}
	}
	while (!isStackEmpty())
	{
		free(pop()); // 스택에 저장된 동적할당된 메모리 모두 반환
	}

	fclose(fp4);
	fclose(fp4w);
	system("notepad.exe 04.txt");

	return 0;
}

int isStackEmpty() // 스택이 공백상태인지 판단하는 함수
{
	if (top == NULL) return 1; // 스택이 공백상태면 '1'반환
	else return 0;
}

void push(char* wp, double item, double k, double bc) // 스택에 구간, 거리, 소모계수, 배터리 소모량을 저장하는 함수
{
	SLL4* temp = (SLL4*)malloc(sizeof(SLL4));

	strcpy(temp->point, wp);
	temp->d = item;
	temp->k = k;
	temp->batteryCon = bc;
	temp->next = top; 
	top = temp;
}

SLL4* pop()
{
	SLL4* temp = top;

	if (isStackEmpty()) // 스택이 공백상태인지 판단
	{
		printf("스택이 비었습니다. \n");
		return 0;
	}
	else
	{
		top = temp->next; // top을 다음 노드로 이동
		return temp;
	}
}

char* navigate(SLL4* t)
{
	char* nav = (char*)malloc(20 * MAX * sizeof(char));
	nav[0] = '\0';

	SLL4* curr = t;
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
