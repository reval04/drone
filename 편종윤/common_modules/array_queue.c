#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf 등
#include "array_queue.h"          // 배열 큐 헤더 포함

/* ════════════════════════════════════════════════════════════
   AQ_init : 원형 큐 초기화
   front와 rear를 0으로 설정 → 빈 큐 상태
   ════════════════════════════════════════════════════════════ */
void AQ_init(ArrayQueue* q) {
    q->front = 0;   // front 인덱스를 0으로 초기화
    q->rear  = 0;   // rear 인덱스를 0으로 초기화
}

/* ════════════════════════════════════════════════════════════
   AQ_isFull : 큐가 가득 찼는지 확인
   (rear + 1) % AQ_MAX_SIZE == front 이면 가득 찬 상태
   ════════════════════════════════════════════════════════════ */
int AQ_isFull(ArrayQueue* q) {
    return (q->rear + 1) % AQ_MAX_SIZE == q->front;   // 원형 조건으로 가득 참 확인
}

/* ════════════════════════════════════════════════════════════
   AQ_isEmpty : 큐가 비어있는지 확인
   front == rear 이면 빈 상태
   ════════════════════════════════════════════════════════════ */
int AQ_isEmpty(ArrayQueue* q) {
    return q->front == q->rear;   // front와 rear가 같으면 1(true) 반환
}

/* ════════════════════════════════════════════════════════════
   AQ_enqueue : 큐의 rear에 값 삽입
   rear를 1 증가(원형)시키고 해당 위치에 val 저장
   ════════════════════════════════════════════════════════════ */
void AQ_enqueue(ArrayQueue* q, element val) {
    if (AQ_isFull(q)) {                        // 큐가 가득 찬 경우
        printf("[AQ 오류] 큐 오버플로우!\n"); // 오류 메시지 출력
        return;                                // 함수 종료
    }
    q->rear = (q->rear + 1) % AQ_MAX_SIZE;   // rear를 원형으로 1 증가
    q->queue[q->rear] = val;                  // 해당 위치에 값 저장
}

/* ════════════════════════════════════════════════════════════
   AQ_dequeue : 큐의 front 값 꺼내기
   front를 1 증가(원형)시키고 해당 위치 값 반환
   ════════════════════════════════════════════════════════════ */
element AQ_dequeue(ArrayQueue* q) {
    if (AQ_isEmpty(q)) {                       // 큐가 비어있는 경우
        printf("[AQ 오류] 큐 언더플로우!\n"); // 오류 메시지 출력
        return (element)0;                    // 기본값 반환
    }
    q->front = (q->front + 1) % AQ_MAX_SIZE; // front를 원형으로 1 증가
    return q->queue[q->front];               // 해당 위치 값 반환
}

/* ════════════════════════════════════════════════════════════
   AQ_front : front 값 조회 (제거 없음)
   ════════════════════════════════════════════════════════════ */
element AQ_front(ArrayQueue* q) {
    if (AQ_isEmpty(q)) {                // 큐가 비어있는 경우
        printf("[AQ 오류] 빈 큐!\n");  // 오류 메시지 출력
        return (element)0;              // 기본값 반환
    }
    return q->queue[(q->front + 1) % AQ_MAX_SIZE]; // front 다음 위치 값 반환
}

/* ════════════════════════════════════════════════════════════
   AQ_rear : rear 직전 값 조회 (마지막으로 삽입된 값)
   ════════════════════════════════════════════════════════════ */
element AQ_rear(ArrayQueue* q) {
    if (AQ_isEmpty(q)) {                // 큐가 비어있는 경우
        printf("[AQ 오류] 빈 큐!\n");  // 오류 메시지 출력
        return (element)0;              // 기본값 반환
    }
    return q->queue[q->rear];           // rear 위치 값 반환 (마지막 삽입값)
}

/* ════════════════════════════════════════════════════════════
   AQ_size : 현재 저장된 요소 수 반환
   ════════════════════════════════════════════════════════════ */
int AQ_size(ArrayQueue* q) {
    return (q->rear - q->front + AQ_MAX_SIZE) % AQ_MAX_SIZE; // 원형 크기 계산
}

/* ════════════════════════════════════════════════════════════
   AQ_print : 큐 전체 내용 출력 (front → rear 순서)
   ════════════════════════════════════════════════════════════ */
void AQ_print(ArrayQueue* q) {
    printf("[ArrayQueue] 크기=%d | ", AQ_size(q)); // 크기 출력
    printf("front → ");                             // 방향 표시
    int idx = (q->front + 1) % AQ_MAX_SIZE;        // front 다음부터 시작
    int cnt = AQ_size(q);                           // 전체 요소 수
    for (int i = 0; i < cnt; i++) {                 // 요소 수만큼 반복
        printf("%d ", (int)q->queue[idx]);          // 요소 출력
        idx = (idx + 1) % AQ_MAX_SIZE;             // 원형으로 다음 인덱스 이동
    }
    printf("← rear\n");                            // rear 방향 표시
}

/* ════════════════════════════════════════════════════════════
   AQ_clear : 큐 전체 비우기
   front와 rear를 모두 0으로 초기화
   ════════════════════════════════════════════════════════════ */
void AQ_clear(ArrayQueue* q) {
    q->front = 0;   // front를 0으로 초기화
    q->rear  = 0;   // rear를 0으로 초기화
}
