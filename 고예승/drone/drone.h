#ifndef DRONE_H
#define DRONE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX 5
#define MAX_RAND 500
#define MAX_BATTERY 3500

// --- [구조체 정의] ---
typedef struct singleLinkedList {
    char name;
    int x;
    int y;
    struct singleLinkedList* next;
} SLL;

typedef struct singleLinkedListHead {
    SLL* head;
} SLLH;

typedef struct singleLinkedList_4 {
    char point[11];
    double d;
    double k;
    double batteryCon;
    struct singleLinkedList_4* next;
} SLL4;

// --- [인터페이스 파이프라인 함수] ---
void run_step_01();
void run_step_02();
void run_step_03();
void run_step_04();

// --- [리스트 및 알고리즘] ---
SLLH* createSingleLinkedList();
void insertLastNode(SLLH* L, int x, int y, char n);
void bubbleSort(SLLH* L);
void pinPointing(SLLH* L);
void freeLinkedList(SLLH* L);

// --- [계산 및 출력] ---
double distance(SLL* p);
void printDistance(double* d, SLL* t);
void printIndex(FILE* p);

// --- [스택] ---
void push(char* wp, double d, double k, double bc);
SLL4* pop();
int isStackEmpty();
char* navigate(SLL4* t);

#endif