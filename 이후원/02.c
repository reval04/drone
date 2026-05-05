#include "drone.h"

#define MAX 5

void arrangeWayPoint() // 좌표값을 거리순으로 정렬하고 name을 부여하는 단일 연결 리스트 생성 함수
{
	FILE* fp2 = fopen("01.txt", "r");
	SLLH* head = createSingleLinkedList(); // 단일 연결 리스트 생성 함수
	SLL* temp;

	int i, x, y;
	for (i = 0; i < MAX; i++)
	{
		fscanf(fp2, "%8d %8d", &x, &y);
		insertLastNode(head, x, y); // 리스트 마지막에 노드로 삽입하는 함수
	}
	fclose(fp2);
	
	bubbleSort(head); // 노드들을 거리값을 기준으로 오름차순으로 정렬하는 함수
	pinPointing(head); // 노드에 name을 부여하는 함수
	FILE * fp2w = fopen("02.txt", "w");
	temp = head->head; // 첫 번째 노드를 가리키도록 temp 설정

	while (temp != NULL) // 리스트의 끝까지 반복
	{
		fprintf(fp2w, "%8d %8d\n", temp->x, temp->y);
		printf("%8d %8d\n", temp->x, temp->y);
		temp = temp->next; // 다음 노드로 이동
	}
	fclose(fp2w);
	freeLinkedList(head); // 리스트 동적해제 함수
	printf("===================================================\n");
	system("notepad.exe 02.txt");
}

void bubbleSort(SLLH* L) // 노드들을 거리값을 기준으로 오름차순으로 정렬하는 함수
{
	if (L == NULL || L->head == NULL) // 공백 리스트인지 확인
	{
		return;
	}

	SLL* current = L->head;
	int i, j;
	int distance1, distance2;
	for (i = 0; i < MAX - 1; i++)
	{
		current = L->head; // 반복할때 마다 첫 번째 노드를 가리키도록 current 설정
		for (j = 0; j < MAX - 1 - i; j++)
		{
			if (current != NULL && current->next != NULL) // 현재 노드와 다음 노드가 모두 있는지 확인
			{
				distance1 = current->x * current->x + current->y * current->y; // 현재 노드의 거리값 계산
				distance2 = current->next->x * current->next->x + current->next->y * current->next->y;

				if (distance1 > distance2) // 현재 노드의 거리값이 다음 노드의 거리값보다 큰 경우
				{
					int xTemp, yTemp;
					xTemp = current->x; yTemp = current->y;
					current->x = current->next->x; current->y = current->next->y; // 현재 노드의 좌표값을 다음 노드의 좌표값으로 변경
					current->next->x = xTemp; current->next->y = yTemp; // 다음 노드의 좌표값을 현재 노드의 좌표값으로 변경
				}
				current = current->next; // 다음 노드로 이동
			}
		}
	}
}

void pinPointing(SLLH* L) // 노드에 name을 부여하는 함수
{
	SLL* temp = L->head;
	int i = 0;

	while (temp != NULL) // 리스트의 끝까지 반복
	{
		temp->name = 'A' + i; // 노드에 name값을 부여(A, B, C, D, E)
		i++;
		temp = temp->next; // 다음 노드로 이동
	}
}
