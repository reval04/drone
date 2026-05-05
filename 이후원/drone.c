#include "drone.h"

SLLH* createSingleLinkedList() // 단일 연결 리스트 생성 함수
{
	SLLH* h;
	h = (SLLH*)malloc(sizeof(SLLH));
	h->head = NULL;
	return h;
}

void insertLastNode(SLLH* L, int x, int y) // 리스트 마지막에 노드로 삽입하는 함수
{
	int count = 1;
	SLL* newNode;
	SLL* temp;
	newNode = (SLL*)malloc(sizeof(SLL)); // 드론 좌표 구조체 동적생성
	newNode->x = x;
	newNode->y = y;
	newNode->next = NULL;
	if (L->head == NULL) { // 공백 리스트인지 판단
		newNode->name = 'A';
		L->head = newNode;
		return;
	}

	temp = L->head;
	while (temp->next != NULL) // temp를 다움노드가 NULL까지 순환
	{
		temp = temp->next;
		count++;
	}
	temp->next = newNode;
	newNode->name = 'A' + count;
	count++;
}

void freeLinkedList(SLLH* L) // 리스트 동적해제 함수
{
	SLL* p;
	while (L->head != NULL) {
		p = L->head;
		L->head = L->head->next; // p가 다음노드를 가리키도록 명령
		free(p); // 노드 동적 해제
		p = NULL;
	}
}
