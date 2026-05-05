#define _CRT_SECURE_NO_WARNINGS
#include "drone.h"

// --- 연결 리스트 구현 ---
LinkedList* createList() {
    LinkedList* L = (LinkedList*)malloc(sizeof(LinkedList));
    if (L == NULL) return NULL; // C6011 방지
    L->head = NULL;
    L->size = 0;
    return L;
}

void insertLast(LinkedList* L, char id, int x, int y) {
    if (L == NULL) return; // 방어적 코드

    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL) return; // C6011 방지

    newNode->id = id;
    newNode->x = x;
    newNode->y = y;
    newNode->link = NULL;

    if (L->head == NULL) {
        L->head = newNode;
    }
    else {
        ListNode* temp = L->head;
        while (temp->link != NULL) {
            temp = temp->link;
        }
        temp->link = newNode;
    }
    L->size++;
}

void freeList(LinkedList* L) {
    if (L == NULL) return;
    ListNode* p = L->head;
    while (p != NULL) {
        ListNode* temp = p;
        p = p->link;
        free(temp);
    }
    free(L);
}

// --- 스택 구현 ---
Stack* createStack() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (s == NULL) return NULL; // C6011 방지
    s->top = NULL;
    s->size = 0;
    return s;
}

void push(Stack* s, Position p) {
    if (s == NULL) return; // 방어적 코드

    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) return; // C6011 방지

    newNode->data = p;
    newNode->link = s->top;
    s->top = newNode;
    s->size++;
}

Position pop(Stack* s) {
    if (s == NULL || isStackEmpty(s)) {
        Position empty = { '\0', 0, 0, 0.0, NORMAL };
        return empty;
    }
    StackNode* temp = s->top;
    Position data = temp->data;
    s->top = temp->link;
    free(temp);
    s->size--;
    return data;
}

int isStackEmpty(Stack* s) {
    if (s == NULL) return 1;
    return s->top == NULL;
}

void freeStack(Stack* s) {
    if (s == NULL) return;
    while (!isStackEmpty(s)) {
        pop(s);
    }
    free(s);
}