#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf, FILE, fopen 등
#include <stdlib.h>               // malloc, free 동적 메모리 함수
#include "droneStack.h"           // 드론 스택 헤더 포함

/* ════════════════════════════════════════════════════════════
   createStack : 빈 스택을 동적으로 생성하여 반환
   ════════════════════════════════════════════════════════════ */
Stack* createStack(void) {
    Stack* s    = (Stack*)malloc(sizeof(Stack)); // 스택 헤드 구조체 메모리 할당
    s->top  = NULL;                              // 아직 노드가 없으므로 top을 NULL로 초기화
    s->size = 0;                                 // 노드 수 0으로 초기화
    return s;                                    // 생성된 스택 포인터 반환
}

/* ════════════════════════════════════════════════════════════
   freeStack : 스택의 모든 노드와 헤드 구조체 메모리 해제
   ════════════════════════════════════════════════════════════ */
void freeStack(Stack* s) {
    stackNode* temp;              // 해제할 노드를 임시 보관할 포인터
    while (s->top != NULL) {      // 스택에 노드가 남아있는 동안 반복
        temp   = s->top;          // 현재 top 노드를 temp에 저장
        s->top = s->top->link;    // top을 그 아래 노드로 이동
        free(temp);               // 이전 top 노드 메모리 해제
    }
    free(s);                      // 스택 헤드 구조체 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   isStackEmpty : 스택이 비어있는지 확인
   반환값: 비어있으면 1, 아니면 0
   ════════════════════════════════════════════════════════════ */
int isStackEmpty(Stack* s) {
    return (s->top == NULL);   // top이 NULL이면 빈 스택 → 1 반환, 아니면 0
}

/* ════════════════════════════════════════════════════════════
   push : 스택 top에 새 위치 정보 노드 삽입
   ════════════════════════════════════════════════════════════ */
void push(Stack* s, Position item) {
    stackNode* temp = (stackNode*)malloc(sizeof(stackNode)); // 새 노드 메모리 할당
    temp->data = item;    // 새 노드에 위치 데이터 저장
    temp->link = s->top;  // 새 노드의 link를 현재 top으로 연결 (기존 스택 위에 쌓기)
    s->top     = temp;    // top 포인터를 새 노드로 갱신
    s->size++;            // 스택 크기 1 증가
}

/* ════════════════════════════════════════════════════════════
   pop : 스택 top에서 위치 정보를 꺼내고 노드 제거
   반환값: 꺼낸 Position (비어있으면 step=-1인 더미 반환)
   ════════════════════════════════════════════════════════════ */
Position pop(Stack* s) {
    Position item;                          // 반환할 위치 데이터 선언
    if (isStackEmpty(s)) {                  // 스택이 비어있으면
        printf("\n스택이 비어있습니다!\n"); // 오류 메시지 출력
        item.step = -1; item.x = 0;         // 더미 값 설정
        item.y = 0; item.battery = 0;       // 더미 값 설정
        return item;                        // 더미 데이터 반환
    }
    stackNode* temp = s->top;   // top 노드를 temp에 저장
    item    = temp->data;       // top 노드의 데이터를 item에 복사
    s->top  = temp->link;       // top을 그 아래 노드로 이동
    free(temp);                 // 꺼낸 노드 메모리 해제
    s->size--;                  // 스택 크기 1 감소
    return item;                // 꺼낸 위치 데이터 반환
}

/* ════════════════════════════════════════════════════════════
   peek : 스택 top의 위치 정보 조회 (노드 제거 없음)
   ════════════════════════════════════════════════════════════ */
Position peek(Stack* s) {
    if (isStackEmpty(s)) {                   // 스택이 비어있으면
        printf("\n스택이 비어있습니다!\n"); // 오류 메시지 출력
        Position empty = {-1, 0, 0, 0};     // 더미 Position 초기화
        return empty;                        // 더미 데이터 반환
    }
    return s->top->data;   // top 노드의 데이터를 복사하여 반환 (제거 없음)
}

/* ════════════════════════════════════════════════════════════
   printStack : 스택 내용을 top→bottom 방향으로 화면 출력
   ════════════════════════════════════════════════════════════ */
void printStack(Stack* s) {
    stackNode* p = s->top;                              // 순회 포인터를 top에 설정
    printf("STACK (top→bottom, 총 %d개):\n", s->size); // 헤더 출력
    while (p != NULL) {                                 // top부터 bottom까지 순회
        printf("  Step:%3d  위치:(%4d,%4d)  배터리:%3d%%\n",  // 각 노드 데이터 출력
               p->data.step, p->data.x, p->data.y, p->data.battery);
        p = p->link;                                    // 아래 노드로 이동
    }
}

/* ════════════════════════════════════════════════════════════
   saveStack : 스택 내용(top→bottom)을 파일에 이어쓰기(append)
   ════════════════════════════════════════════════════════════ */
void saveStack(Stack* s, const char* filename) {
    FILE* fp = fopen(filename, "a");    // 추가(append) 모드로 파일 열기
    if (!fp) {                          // 파일 열기 실패 확인
        printf("파일 열기 실패: %s\n", filename); // 오류 메시지
        return;                         // 함수 종료
    }
    stackNode* p = s->top;             // 순회 포인터를 top에 설정
    while (p != NULL) {                // 모든 노드 순회
        fprintf(fp, "%d %d %d %d\n",   // step, x, y, battery를 파일에 기록
                p->data.step, p->data.x, p->data.y, p->data.battery);
        p = p->link;                   // 다음(아래) 노드로 이동
    }
    fclose(fp);                        // 파일 닫기
}
