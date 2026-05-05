/* ════════════════════════════════════════════════════════════
   array_stack.h  —  배열 기반 스택 모듈 헤더
   ─────────────────────────────────────────────────────────
   정적 배열을 사용하는 스택 구현.
   element 타입은 상단 typedef를 수정하여 변경 가능.
   ════════════════════════════════════════════════════════════ */

#pragma once          // 헤더 중복 포함 방지
#include <stdio.h>    // printf 등

/* ── element 타입 정의 (공통 정의가 없을 때만 정의) ── */
#ifndef ELEMENT_TYPE_DEFINED     // 아직 element가 정의되지 않았으면
#define ELEMENT_TYPE_DEFINED     // 정의 완료 플래그
typedef int element;             // 기본 element 타입: int
#endif

#define AS_MAX_SIZE  128   // 배열 스택 최대 저장 용량

/* ──────────────────────────────────────────────────────────
   ArrayStack 구조체
   배열과 top 인덱스로 구성되는 스택
   ────────────────────────────────────────────────────────── */
typedef struct {
    element stack[AS_MAX_SIZE];  // 데이터 저장 배열
    int     top;                 // top 인덱스 (-1 = 비어있음)
} ArrayStack;                    // 구조체 별칭: ArrayStack

/* ── 스택 기본 연산 ── */
void    AS_init    (ArrayStack* s);              // 스택 초기화 (top = -1)
int     AS_isFull  (ArrayStack* s);              // 스택이 가득 찼으면 1
int     AS_isEmpty (ArrayStack* s);              // 스택이 비어있으면 1
void    AS_push    (ArrayStack* s, element val); // top에 val 삽입
element AS_pop     (ArrayStack* s);              // top 값 꺼내기 (제거)
element AS_peek    (ArrayStack* s);              // top 값 조회 (제거 없음)
int     AS_size    (ArrayStack* s);              // 현재 저장된 요소 수 반환
void    AS_print   (ArrayStack* s);              // 스택 전체 출력 (bottom→top)
void    AS_clear   (ArrayStack* s);              // 스택 전체 비우기
