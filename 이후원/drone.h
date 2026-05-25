#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX 5
#define MAX_RAND 500
#define MAX_BATTERY 3000
#define QUEUE_CAP 10
#define LAMBDA 5.0
#define MU 7.0


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

// Step 1 — 충전 대기열 (원형 큐)
typedef struct chargeQueue {
    char  droneName[QUEUE_CAP][12];   // 드론(경로) 이름
    double batteryUsed[QUEUE_CAP];    // 누적 배터리 소모량
    int   front, rear, size;
} ChargeQueue;

// Step 2 — 위험 구간 분석 BST
typedef struct bstNode {
    char   segment[12];        // 구간명 (예: "AB")
    double consumption;        // 배터리 소모량
    struct bstNode* left;
    struct bstNode* right;
} BSTNode;

// Step 3 — 웨이포인트 이진 트리
typedef struct treeNode {
    char name;                 // 'A' ~ 'E'
    struct treeNode* left;
    struct treeNode* right;
} TreeNode;

// --- [함수 선언] ---
// 01, 02, 03 실행 함수
void makeWayPoint();
void arrangeWayPoint();
void makeDistancePerBatteryTable();

// 리스트 및 알고리즘 (func.c)
SLLH* createSingleLinkedList();
void insertLastNode(SLLH* L, int x, int y, char n);
void bubbleSort(SLLH* L);
void pinPointing(SLLH* L);
void freeLinkedList(SLLH* L);

// 계산 및 출력 (func.c)
double distance(SLL* p);
void printDistance(double* d, SLL* t);
void printIndex(FILE* p);

// 스택 (func.c)
void push(char* wp, double d, double k, double bc);
SLL4* pop();
int isStackEmpty();
char* navigate(SLL4* t);

// ===== 05.c 함수 원형 =====
// Step 0
void step0_queueing(double lambda, double mu);
// Step 1
void queueInit(ChargeQueue* q);
int  enqueue(ChargeQueue* q, char* name, double used);
int  dequeue(ChargeQueue* q, char* outName, double* outUsed);
// Step 2
BSTNode* bstInsert(BSTNode* root, char* seg, double con);
void     bstInorder(BSTNode* root);
void     bstFree(BSTNode* root);
// Step 3
TreeNode* treeNew(char name);
void      preorder(TreeNode* node);
void      postorder(TreeNode* node);
void      treeFree(TreeNode* node);

void runStep0123();
