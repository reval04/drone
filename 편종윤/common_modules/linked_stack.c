#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf 등
#include <stdlib.h>               // malloc, free
#include "linked_stack.h"         // 연결 스택 헤더 포함

/* ════════════════════════════════════════════════════════════
   LS_create : 빈 연결 스택 생성
   동적으로 LinkedStack 구조체를 할당하고 초기화
   ════════════════════════════════════════════════════════════ */
LinkedStack* LS_create(void) {
    LinkedStack* s = (LinkedStack*)malloc(sizeof(LinkedStack)); // 스택 구조체 할당
    s->top  = NULL;   // top 포인터를 NULL로 초기화 (빈 스택)
    s->size = 0;      // 요소 수를 0으로 초기화
    return s;         // 생성된 스택 반환
}

/* ════════════════════════════════════════════════════════════
   LS_free : 스택 전체 메모리 해제
   모든 노드를 순서대로 해제한 후 스택 구조체도 해제
   ════════════════════════════════════════════════════════════ */
void LS_free(LinkedStack* s) {
    LS_clear(s);   // 모든 노드 먼저 해제
    free(s);       // 스택 구조체 해제
}

/* ════════════════════════════════════════════════════════════
   LS_isEmpty : 스택이 비어있는지 확인
   top이 NULL이면 빈 상태
   ════════════════════════════════════════════════════════════ */
int LS_isEmpty(LinkedStack* s) {
    return s->top == NULL;   // top이 NULL이면 1(true) 반환
}

/* ════════════════════════════════════════════════════════════
   LS_push : 스택에 값 삽입
   새 노드를 동적 할당하여 top에 연결
   ════════════════════════════════════════════════════════════ */
void LS_push(LinkedStack* s, element val) {
    LS_Node* newNode = (LS_Node*)malloc(sizeof(LS_Node)); // 새 노드 할당
    if (!newNode) {                                        // 할당 실패 시
        printf("[LS 오류] 메모리 할당 실패!\n");          // 오류 출력
        return;                                            // 함수 종료
    }
    newNode->data = val;    // 데이터 저장
    newNode->link = s->top; // 현재 top을 새 노드의 아래로 연결
    s->top        = newNode;// top을 새 노드로 업데이트
    s->size++;              // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   LS_pop : 스택에서 값 꺼내기
   top 노드 값을 반환하고 노드를 해제, top을 이동
   ════════════════════════════════════════════════════════════ */
element LS_pop(LinkedStack* s) {
    if (LS_isEmpty(s)) {                       // 스택이 비어있으면
        printf("[LS 오류] 스택 언더플로우!\n"); // 오류 출력
        return (element)0;                     // 기본값 반환
    }
    LS_Node* del  = s->top;      // 삭제할 top 노드 포인터
    element  val  = del->data;   // top 노드의 데이터 저장
    s->top        = del->link;   // top을 다음 노드로 이동
    free(del);                   // 이전 top 노드 메모리 해제
    s->size--;                   // 요소 수 감소
    return val;                  // 꺼낸 값 반환
}

/* ════════════════════════════════════════════════════════════
   LS_peek : top 값 조회 (제거 없음)
   ════════════════════════════════════════════════════════════ */
element LS_peek(LinkedStack* s) {
    if (LS_isEmpty(s)) {                // 스택이 비어있으면
        printf("[LS 오류] 빈 스택!\n"); // 오류 출력
        return (element)0;              // 기본값 반환
    }
    return s->top->data;   // top 노드의 데이터 반환 (제거 없음)
}

/* ════════════════════════════════════════════════════════════
   LS_size : 현재 저장된 요소 수 반환
   ════════════════════════════════════════════════════════════ */
int LS_size(LinkedStack* s) {
    return s->size;   // size 필드 반환
}

/* ════════════════════════════════════════════════════════════
   LS_print : 스택 전체 내용 출력 (top → bottom 순서)
   ════════════════════════════════════════════════════════════ */
void LS_print(LinkedStack* s) {
    printf("[LinkedStack] 크기=%d | ", s->size); // 크기 출력
    printf("top → ");                             // 방향 표시
    LS_Node* p = s->top;                          // 순회 포인터를 top으로 설정
    while (p != NULL) {                           // 끝까지 순회
        printf("%d ", (int)p->data);              // 각 노드 데이터 출력
        p = p->link;                              // 다음 노드로 이동
    }
    printf("← bottom\n");                        // bottom 방향 표시
}

/* ════════════════════════════════════════════════════════════
   LS_clear : 스택 전체 비우기
   모든 노드를 해제하고 top을 NULL, size를 0으로 초기화
   ════════════════════════════════════════════════════════════ */
void LS_clear(LinkedStack* s) {
    while (!LS_isEmpty(s))   // 노드가 남은 동안 반복
        LS_pop(s);           // pop으로 하나씩 꺼내며 해제
}
