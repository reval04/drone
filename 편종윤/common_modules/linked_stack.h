/* ════════════════════════════════════════════════════════════
   linked_stack.h  —  연결 리스트 기반 스택 모듈 헤더
   ─────────────────────────────────────────────────────────
   동적 메모리 할당을 사용하는 스택 구현.
   크기 제한 없이 필요한 만큼 노드를 할당.
   사용 후 반드시 LS_free()로 메모리 해제 필요.
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
   LS_Node 구조체 (연결 스택 노드)
   ────────────────────────────────────────────────────────── */
typedef struct LS_Node {
    element          data;   // 저장할 데이터
    struct LS_Node*  link;   // 아래 노드(이전 위치)를 가리키는 포인터
} LS_Node;                   // 구조체 별칭: LS_Node

/* ──────────────────────────────────────────────────────────
   LinkedStack 구조체
   ────────────────────────────────────────────────────────── */
typedef struct {
    LS_Node* top;   // 스택 top 노드 포인터
    int      size;  // 현재 저장된 노드 수
} LinkedStack;      // 구조체 별칭: LinkedStack

/* ── 스택 기본 연산 ── */
LinkedStack* LS_create  (void);                   // 빈 스택 생성 (동적 할당)
void         LS_free    (LinkedStack* s);          // 스택 전체 메모리 해제
int          LS_isEmpty (LinkedStack* s);          // 스택이 비어있으면 1
void         LS_push    (LinkedStack* s, element val); // top에 val 삽입
element      LS_pop     (LinkedStack* s);          // top 값 꺼내기 (제거)
element      LS_peek    (LinkedStack* s);          // top 값 조회 (제거 없음)
int          LS_size    (LinkedStack* s);          // 현재 저장된 요소 수 반환
void         LS_print   (LinkedStack* s);          // 스택 전체 출력 (top→bottom)
void         LS_clear   (LinkedStack* s);          // 스택 전체 비우기 (노드 해제)
