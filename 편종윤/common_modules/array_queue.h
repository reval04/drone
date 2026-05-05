/* ════════════════════════════════════════════════════════════
   array_queue.h  —  배열 기반 원형 큐 모듈 헤더
   ─────────────────────────────────────────────────────────
   원형 배열(circular array)로 구현한 큐.
   front/rear 인덱스가 배열을 순환하며 공간을 재사용.
   ════════════════════════════════════════════════════════════ */

#pragma once          // 헤더 중복 포함 방지
#include <stdio.h>    // printf 등

/* ── element 타입 정의 ── */
#ifndef ELEMENT_TYPE_DEFINED
#define ELEMENT_TYPE_DEFINED
typedef int element;   // 기본 element 타입: int
#endif

#define AQ_MAX_SIZE  128   // 원형 큐 최대 저장 용량

/* ──────────────────────────────────────────────────────────
   ArrayQueue 구조체
   front: 꺼낼 위치 인덱스, rear: 넣을 위치 인덱스
   (front == rear) → 빈 상태
   ────────────────────────────────────────────────────────── */
typedef struct {
    element queue[AQ_MAX_SIZE]; // 데이터 저장 배열
    int     front;              // 큐 앞 인덱스 (dequeue 위치)
    int     rear;               // 큐 뒤 인덱스 (enqueue 위치)
} ArrayQueue;                   // 구조체 별칭: ArrayQueue

/* ── 큐 기본 연산 ── */
void    AQ_init    (ArrayQueue* q);               // 큐 초기화 (front = rear = 0)
int     AQ_isFull  (ArrayQueue* q);               // 큐가 가득 찼으면 1
int     AQ_isEmpty (ArrayQueue* q);               // 큐가 비어있으면 1
void    AQ_enqueue (ArrayQueue* q, element val);  // rear에 val 삽입
element AQ_dequeue (ArrayQueue* q);               // front 값 꺼내기 (제거)
element AQ_front   (ArrayQueue* q);               // front 값 조회 (제거 없음)
element AQ_rear    (ArrayQueue* q);               // rear 직전 값 조회 (제거 없음)
int     AQ_size    (ArrayQueue* q);               // 현재 저장된 요소 수 반환
void    AQ_print   (ArrayQueue* q);               // 큐 전체 출력 (front→rear)
void    AQ_clear   (ArrayQueue* q);               // 큐 전체 비우기
