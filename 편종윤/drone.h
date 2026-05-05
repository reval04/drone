#ifndef DRONE_H
#define DRONE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// 드론 상태 정의
typedef enum { NORMAL, ERROR_BATT, FAILURE, RECOVERY } DroneState;

// --- 연결 리스트 구조체 (경로 저장용) ---
typedef struct ListNode {
    char id;
    int x;
    int y;
    struct ListNode* link;
} ListNode;

typedef struct {
    ListNode* head;
    int size;
} LinkedList;

// --- 스택 구조체 (역추적 복귀용) ---
typedef struct {
    char id;
    int x;
    int y;
    double cum_consumed;
    DroneState state;
} Position;

typedef struct StackNode {
    Position data;
    struct StackNode* link;
} StackNode;

typedef struct {
    StackNode* top;
    int size;
} Stack;

// 자료구조 함수 원형
LinkedList* createList();
void insertLast(LinkedList* L, char id, int x, int y);
void freeList(LinkedList* L);

Stack* createStack();
void push(Stack* s, Position p);
Position pop(Stack* s);
int isStackEmpty(Stack* s);
void freeStack(Stack* s);

// 파이프라인 함수 원형
void run_step_01();
void run_step_02();
void run_step_03();
void run_step_04();

#endif