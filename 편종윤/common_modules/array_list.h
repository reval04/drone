/* ════════════════════════════════════════════════════════════
   array_list.h  —  배열 기반 순차 리스트 모듈 헤더
   ─────────────────────────────────────────────────────────
   정적 배열을 사용하는 순차 리스트 구현.
   임의 위치 접근(O(1)), 삽입/삭제는 이동 비용(O(n)).
   ════════════════════════════════════════════════════════════ */

#pragma once          // 헤더 중복 포함 방지
#include <stdio.h>    // printf 등

/* ── element 타입 정의 ── */
#ifndef ELEMENT_TYPE_DEFINED
#define ELEMENT_TYPE_DEFINED
typedef int element;   // 기본 element 타입: int
#endif

#define AL_MAX_SIZE  256   // 순차 리스트 최대 저장 용량

/* ──────────────────────────────────────────────────────────
   ArrayList 구조체
   배열과 현재 요소 수(size)로 구성
   ────────────────────────────────────────────────────────── */
typedef struct {
    element list[AL_MAX_SIZE];  // 데이터 저장 배열
    int     size;               // 현재 저장된 요소 수
} ArrayList;                    // 구조체 별칭: ArrayList

/* ── 리스트 기본 연산 ── */
void    AL_init        (ArrayList* L);                    // 리스트 초기화 (size = 0)
int     AL_isFull      (ArrayList* L);                    // 가득 찼으면 1
int     AL_isEmpty     (ArrayList* L);                    // 비어있으면 1
void    AL_insertAt    (ArrayList* L, int pos, element val); // pos 위치에 val 삽입
void    AL_insertLast  (ArrayList* L, element val);       // 맨 끝에 val 삽입
void    AL_deleteAt    (ArrayList* L, int pos);           // pos 위치 요소 삭제
int     AL_indexOf     (ArrayList* L, element val);       // val의 위치 반환 (-1=없음)
element AL_get         (ArrayList* L, int pos);           // pos 위치 요소 반환
void    AL_set         (ArrayList* L, int pos, element val); // pos 위치 요소 변경
int     AL_size        (ArrayList* L);                    // 현재 요소 수 반환
void    AL_print       (ArrayList* L);                    // 리스트 전체 출력
void    AL_sortAsc     (ArrayList* L);                    // 오름차순 버블 정렬
void    AL_sortDesc    (ArrayList* L);                    // 내림차순 버블 정렬
void    AL_clear       (ArrayList* L);                    // 리스트 전체 비우기
