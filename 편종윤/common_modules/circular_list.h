/* ════════════════════════════════════════════════════════════
   circular_list.h  —  원형 연결 리스트 모듈 헤더
   ─────────────────────────────────────────────────────────
   마지막 노드의 link가 head를 가리키는 원형(순환) 리스트.
   순환 탐색이나 라운드 로빈 처리에 적합.
   사용 후 반드시 CL_free()로 메모리 해제 필요.
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
   CL_Node 구조체 (원형 연결 리스트 노드)
   마지막 노드의 link는 head를 가리킴
   ────────────────────────────────────────────────────────── */
typedef struct CL_Node {
    element          data;   // 저장할 데이터
    struct CL_Node*  link;   // 다음 노드를 가리키는 포인터 (마지막→head)
} CL_Node;                   // 구조체 별칭: CL_Node

/* ──────────────────────────────────────────────────────────
   CircularList 구조체
   head: 첫 번째 노드 포인터
   ────────────────────────────────────────────────────────── */
typedef struct {
    CL_Node* head;   // 리스트 첫 번째 노드 포인터
    int      size;   // 현재 저장된 노드 수
} CircularList;      // 구조체 별칭: CircularList

/* ── 리스트 기본 연산 ── */
CircularList* CL_create      (void);                        // 빈 리스트 생성
void          CL_free        (CircularList* CL);            // 전체 메모리 해제
int           CL_isEmpty     (CircularList* CL);            // 비어있으면 1
void          CL_insertFront (CircularList* CL, element val); // head 앞에 삽입
void          CL_insertLast  (CircularList* CL, element val); // 맨 끝에 삽입
void          CL_deleteFirst (CircularList* CL);            // head 노드 삭제
void          CL_deleteVal   (CircularList* CL, element val); // 특정 값 첫 노드 삭제
CL_Node*      CL_search      (CircularList* CL, element val); // 값 탐색 (포인터 반환)
int           CL_size        (CircularList* CL);            // 현재 요소 수 반환
void          CL_print       (CircularList* CL);            // 리스트 전체 출력 (한 바퀴)
void          CL_sortAsc     (CircularList* CL);            // 오름차순 버블 정렬
void          CL_clear       (CircularList* CL);            // 전체 비우기 (노드 해제)
