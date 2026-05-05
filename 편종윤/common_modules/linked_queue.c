#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf 등
#include <stdlib.h>               // malloc, free
#include "linked_queue.h"         // 연결 큐 헤더 포함

/* ════════════════════════════════════════════════════════════
   LQ_create : 빈 연결 큐 생성
   LinkedQueue 구조체를 동적 할당하고 초기화
   ════════════════════════════════════════════════════════════ */
LinkedQueue* LQ_create(void) {
    LinkedQueue* q = (LinkedQueue*)malloc(sizeof(LinkedQueue)); // 큐 구조체 할당
    q->front = NULL;   // front 포인터를 NULL로 초기화
    q->rear  = NULL;   // rear 포인터를 NULL로 초기화
    q->size  = 0;      // 요소 수를 0으로 초기화
    return q;          // 생성된 큐 반환
}

/* ════════════════════════════════════════════════════════════
   LQ_free : 큐 전체 메모리 해제
   모든 노드를 해제한 후 큐 구조체도 해제
   ════════════════════════════════════════════════════════════ */
void LQ_free(LinkedQueue* q) {
    LQ_clear(q);   // 모든 노드 먼저 해제
    free(q);       // 큐 구조체 해제
}

/* ════════════════════════════════════════════════════════════
   LQ_isEmpty : 큐가 비어있는지 확인
   front가 NULL이면 빈 상태
   ════════════════════════════════════════════════════════════ */
int LQ_isEmpty(LinkedQueue* q) {
    return q->front == NULL;   // front가 NULL이면 1(true) 반환
}

/* ════════════════════════════════════════════════════════════
   LQ_enqueue : 큐의 rear에 값 삽입
   새 노드를 동적 할당하여 rear 뒤에 연결
   ════════════════════════════════════════════════════════════ */
void LQ_enqueue(LinkedQueue* q, element val) {
    LQ_Node* newNode = (LQ_Node*)malloc(sizeof(LQ_Node)); // 새 노드 할당
    if (!newNode) {                                         // 할당 실패 시
        printf("[LQ 오류] 메모리 할당 실패!\n");           // 오류 출력
        return;                                             // 함수 종료
    }
    newNode->data = val;    // 데이터 저장
    newNode->link = NULL;   // 새 노드는 마지막이므로 link는 NULL

    if (LQ_isEmpty(q)) {    // 큐가 비어있으면
        q->front = newNode; // front와 rear 모두 새 노드 가리킴
        q->rear  = newNode; // rear도 새 노드
    } else {                // 이미 노드가 있으면
        q->rear->link = newNode; // 현재 rear의 다음을 새 노드로 연결
        q->rear       = newNode; // rear를 새 노드로 업데이트
    }
    q->size++;   // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   LQ_dequeue : 큐의 front 값 꺼내기
   front 노드 값을 반환하고 노드를 해제, front를 다음으로 이동
   ════════════════════════════════════════════════════════════ */
element LQ_dequeue(LinkedQueue* q) {
    if (LQ_isEmpty(q)) {                       // 큐가 비어있으면
        printf("[LQ 오류] 큐 언더플로우!\n"); // 오류 출력
        return (element)0;                     // 기본값 반환
    }
    LQ_Node* del  = q->front;      // 삭제할 front 노드 포인터
    element  val  = del->data;     // front 노드의 데이터 저장
    q->front      = del->link;     // front를 다음 노드로 이동
    if (q->front == NULL)          // front가 NULL이 되면 (큐가 빔)
        q->rear = NULL;            // rear도 NULL로 초기화
    free(del);                     // 이전 front 노드 메모리 해제
    q->size--;                     // 요소 수 감소
    return val;                    // 꺼낸 값 반환
}

/* ════════════════════════════════════════════════════════════
   LQ_front : front 값 조회 (제거 없음)
   ════════════════════════════════════════════════════════════ */
element LQ_front(LinkedQueue* q) {
    if (LQ_isEmpty(q)) {                // 큐가 비어있으면
        printf("[LQ 오류] 빈 큐!\n");  // 오류 출력
        return (element)0;              // 기본값 반환
    }
    return q->front->data;   // front 노드 데이터 반환 (제거 없음)
}

/* ════════════════════════════════════════════════════════════
   LQ_rear : rear 값 조회 (마지막으로 삽입된 값, 제거 없음)
   ════════════════════════════════════════════════════════════ */
element LQ_rear(LinkedQueue* q) {
    if (LQ_isEmpty(q)) {                // 큐가 비어있으면
        printf("[LQ 오류] 빈 큐!\n");  // 오류 출력
        return (element)0;              // 기본값 반환
    }
    return q->rear->data;   // rear 노드 데이터 반환 (제거 없음)
}

/* ════════════════════════════════════════════════════════════
   LQ_size : 현재 저장된 요소 수 반환
   ════════════════════════════════════════════════════════════ */
int LQ_size(LinkedQueue* q) {
    return q->size;   // size 필드 반환
}

/* ════════════════════════════════════════════════════════════
   LQ_print : 큐 전체 내용 출력 (front → rear 순서)
   ════════════════════════════════════════════════════════════ */
void LQ_print(LinkedQueue* q) {
    printf("[LinkedQueue] 크기=%d | ", q->size); // 크기 출력
    printf("front → ");                           // 방향 표시
    LQ_Node* p = q->front;                        // 순회 포인터를 front로 설정
    while (p != NULL) {                           // 끝까지 순회
        printf("%d ", (int)p->data);              // 각 노드 데이터 출력
        p = p->link;                              // 다음 노드로 이동
    }
    printf("← rear\n");                          // rear 방향 표시
}

/* ════════════════════════════════════════════════════════════
   LQ_clear : 큐 전체 비우기
   모든 노드를 dequeue로 해제하고 front/rear를 NULL로 초기화
   ════════════════════════════════════════════════════════════ */
void LQ_clear(LinkedQueue* q) {
    while (!LQ_isEmpty(q))   // 노드가 남은 동안 반복
        LQ_dequeue(q);       // dequeue로 하나씩 꺼내며 해제
}
