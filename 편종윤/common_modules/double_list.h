/* ════════════════════════════════════════════════════════════
   double_list.h  —  이중 연결 리스트 모듈 헤더
   ─────────────────────────────────────────────────────────
   llink(←) / rlink(→) 두 포인터를 가진 양방향 연결 리스트.
   앞뒤 방향 순회 가능, 특정 노드 삭제가 O(1).
   사용 후 반드시 DL_free()로 메모리 해제 필요.
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
   DL_Node 구조체 (이중 연결 리스트 노드)
   ────────────────────────────────────────────────────────── */
typedef struct DL_Node {
    element          data;   // 저장할 데이터
    struct DL_Node*  llink;  // 이전 노드를 가리키는 포인터 (←)
    struct DL_Node*  rlink;  // 다음 노드를 가리키는 포인터 (→)
} DL_Node;                   // 구조체 별칭: DL_Node

/* ──────────────────────────────────────────────────────────
   DoubleList 구조체
   head(첫 노드)와 tail(마지막 노드) 포인터 보유
   ────────────────────────────────────────────────────────── */
typedef struct {
    DL_Node* head;   // 리스트 첫 번째 노드 포인터
    DL_Node* tail;   // 리스트 마지막 노드 포인터
    int      size;   // 현재 저장된 노드 수
} DoubleList;        // 구조체 별칭: DoubleList

/* ── 리스트 기본 연산 ── */
DoubleList* DL_create      (void);                        // 빈 리스트 생성
void        DL_free        (DoubleList* L);               // 전체 메모리 해제
int         DL_isEmpty     (DoubleList* L);               // 비어있으면 1
void        DL_insertFront (DoubleList* L, element val);  // head 앞에 삽입
void        DL_insertLast  (DoubleList* L, element val);  // 맨 끝에 삽입
void        DL_insertAfter (DoubleList* L, DL_Node* p,    // 노드 p 뒤에 삽입
                             element val);
void        DL_deleteNode  (DoubleList* L, DL_Node* p);  // 노드 p 삭제 (포인터 직접)
void        DL_deleteFirst (DoubleList* L);               // head 노드 삭제
void        DL_deleteLast  (DoubleList* L);               // tail 노드 삭제
void        DL_deleteVal   (DoubleList* L, element val);  // 특정 값 첫 노드 삭제
DL_Node*    DL_search      (DoubleList* L, element val);  // 값 탐색 (포인터 반환)
int         DL_size        (DoubleList* L);               // 현재 요소 수 반환
void        DL_printFwd    (DoubleList* L);               // 정방향 출력 (head→tail)
void        DL_printBwd    (DoubleList* L);               // 역방향 출력 (tail→head)
void        DL_sortAsc     (DoubleList* L);               // 오름차순 버블 정렬
void        DL_clear       (DoubleList* L);               // 전체 비우기 (노드 해제)
