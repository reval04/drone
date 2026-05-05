/* ════════════════════════════════════════════════════════════
   single_list.h  —  단순 연결 리스트 모듈 헤더
   ─────────────────────────────────────────────────────────
   동적 노드 할당 기반의 단방향(→) 연결 리스트.
   삽입/삭제 O(1)(위치를 알 때), 탐색 O(n).
   사용 후 반드시 SL_free()로 메모리 해제 필요.
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
   SL_Node 구조체 (단순 연결 리스트 노드)
   ────────────────────────────────────────────────────────── */
typedef struct SL_Node {
    element          data;   // 저장할 데이터
    struct SL_Node*  link;   // 다음 노드를 가리키는 포인터
} SL_Node;                   // 구조체 별칭: SL_Node

/* ──────────────────────────────────────────────────────────
   SingleList 구조체
   head 포인터와 size로 구성
   ────────────────────────────────────────────────────────── */
typedef struct {
    SL_Node* head;   // 리스트 첫 번째 노드 포인터
    int      size;   // 현재 저장된 노드 수
} SingleList;        // 구조체 별칭: SingleList

/* ── 리스트 기본 연산 ── */
SingleList* SL_create      (void);                         // 빈 리스트 생성
void        SL_free        (SingleList* L);                // 전체 메모리 해제
int         SL_isEmpty     (SingleList* L);                // 비어있으면 1
void        SL_insertFront (SingleList* L, element val);  // head 앞에 삽입
void        SL_insertLast  (SingleList* L, element val);  // 맨 끝에 삽입
void        SL_insertAt    (SingleList* L, int pos, element val); // pos 위치에 삽입
void        SL_deleteFirst (SingleList* L);                // head 노드 삭제
void        SL_deleteLast  (SingleList* L);                // 마지막 노드 삭제
void        SL_deleteVal   (SingleList* L, element val);  // 특정 값의 첫 노드 삭제
SL_Node*    SL_search      (SingleList* L, element val);  // 값 탐색 (포인터 반환)
element     SL_get         (SingleList* L, int pos);      // pos 위치 값 반환
int         SL_size        (SingleList* L);                // 현재 요소 수 반환
void        SL_print       (SingleList* L);                // 리스트 전체 출력
void        SL_sortAsc     (SingleList* L);                // 오름차순 버블 정렬 (data 교환)
void        SL_reverse     (SingleList* L);                // 리스트 역순 재연결
void        SL_clear       (SingleList* L);                // 전체 비우기 (노드 해제)
