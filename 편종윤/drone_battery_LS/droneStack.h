#pragma once         // 헤더 파일 중복 포함 방지
#include <stdio.h>   // FILE, printf 등 표준 입출력 헤더

/* ──────────────────────────────────────────────────────────
   Position 구조체
   드론이 한 스텝에서의 위치 및 배터리 잔량을 저장한다
   ────────────────────────────────────────────────────────── */
typedef struct {
    int step;     // 비행 스텝 번호 (1부터 순서대로 증가)
    int x;        // 현재 X 좌표 (단위: m, 베이스는 0)
    int y;        // 현재 Y 좌표 (단위: m, 베이스는 0)
    int battery;  // 이 위치에서의 남은 배터리 (0~100%)
} Position;       // 구조체 별칭: Position

/* ──────────────────────────────────────────────────────────
   stackNode 구조체
   연결 리스트 기반 스택의 각 노드
   ────────────────────────────────────────────────────────── */
typedef struct StackNode {
    Position data;           // 이 노드에 저장되는 위치 데이터
    struct StackNode* link;  // 아래 노드(이전에 push된 노드)를 가리키는 포인터
} stackNode;                 // 구조체 별칭: stackNode

/* ──────────────────────────────────────────────────────────
   Stack 구조체
   스택 전체를 관리하는 헤드 구조체
   ────────────────────────────────────────────────────────── */
typedef struct {
    stackNode* top;  // 스택의 top 노드를 가리키는 포인터
    int size;        // 현재 스택에 쌓인 노드 수
} Stack;             // 구조체 별칭: Stack

/* ── 함수 원형 선언 ── */
Stack*   createStack(void);                       // 빈 스택 생성 및 반환
void     freeStack(Stack* s);                     // 스택 전체 메모리 해제
int      isStackEmpty(Stack* s);                  // 스택이 비어있는지 확인 (비면 1)
void     push(Stack* s, Position item);           // 스택 top에 위치 정보 삽입
Position pop(Stack* s);                           // 스택 top에서 위치 정보 꺼내기
Position peek(Stack* s);                          // 스택 top 위치 정보 조회 (제거 없음)
void     printStack(Stack* s);                    // 스택 전체 내용 화면 출력
void     saveStack(Stack* s, const char* filename);// 스택 내용을 파일에 추가(append) 저장
