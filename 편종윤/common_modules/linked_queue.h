/* ════════════════════════════════════════════════════════════
   linked_queue.h  —  연결 리스트 기반 큐 모듈 헤더
   ─────────────────────────────────────────────────────────
   동적 메모리 할당을 사용하는 큐 구현.
   front 포인터로 dequeue, rear 포인터로 enqueue.
   크기 제한 없이 동적으로 확장 가능.
   사용 후 반드시 LQ_free()로 메모리 해제 필요.
   ════════════════════════════════════════════════════════════ */

#pragma once          // 헤더 중복 포함 방지
#include <stdio.h>    // printf 등
#include <stdlib.h>   // malloc, free

/* ── element 타입 정의 ── */
#ifndef ELEMENT_TYPE_DEFINED
#define ELEMENT_TYPE_DEFINED
typedef int element;   // 기본 element 타입: int
#endif

/* ──────────────────────────────────────────────────────────
   LQ_Node 구조체 (연결 큐 노드)
   ────────────────────────────────────────────────────────── */
typedef struct LQ_Node {
    element         data;   // 저장할 데이터
    struct LQ_Node* link;   // 다음 노드를 가리키는 포인터
} LQ_Node;                  // 구조체 별칭: LQ_Node

/* ──────────────────────────────────────────────────────────
   LinkedQueue 구조체
   front: dequeue(꺼낼) 포인터, rear: enqueue(삽입) 포인터
   ────────────────────────────────────────────────────────── */
typedef struct {
    LQ_Node* front;   // 큐 앞 노드 포인터 (꺼낼 위치)
    LQ_Node* rear;    // 큐 뒤 노드 포인터 (삽입 위치)
    int      size;    // 현재 저장된 노드 수
} LinkedQueue;        // 구조체 별칭: LinkedQueue

/* ── 큐 기본 연산 ── */
LinkedQueue* LQ_create  (void);                    // 빈 큐 생성 (동적 할당)
void         LQ_free    (LinkedQueue* q);           // 큐 전체 메모리 해제
int          LQ_isEmpty (LinkedQueue* q);           // 큐가 비어있으면 1
void         LQ_enqueue (LinkedQueue* q, element val); // rear에 val 삽입
element      LQ_dequeue (LinkedQueue* q);           // front 값 꺼내기 (제거)
element      LQ_front   (LinkedQueue* q);           // front 값 조회 (제거 없음)
element      LQ_rear    (LinkedQueue* q);           // rear 값 조회 (제거 없음)
int          LQ_size    (LinkedQueue* q);           // 현재 저장된 요소 수 반환
void         LQ_print   (LinkedQueue* q);           // 큐 전체 출력 (front→rear)
void         LQ_clear   (LinkedQueue* q);           // 큐 전체 비우기 (노드 해제)
