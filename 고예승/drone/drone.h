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


//----------------6장 큐잉이론 추가---------------------
#define MAX_QUEUE 5

typedef struct {
    int droneID;
    double remainingBattery;
} Drone;

typedef struct {
    Drone data[MAX_QUEUE];
    int front, rear;
} Queue;

void initQueue(Queue* q);
int isQueueEmpty(Queue* q);
int isQueueFull(Queue* q);
void enqueue(Queue* q, Drone d);
Drone dequeue(Queue* q);
double checkQueueingTheory(double lambda, double mu);


//----------------7장 트리 추가---------------------
typedef struct TreeNode {
    char name;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createTreeNode(char name);
void preOrder(TreeNode* root);
void postOrder(TreeNode* root);

// 2. 위험 구간 분석 이진 탐색 트리 (BST)
typedef struct BSTNode {
    char section[3];
    double batteryConsumed;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

BSTNode* insertBST(BSTNode* root, char* section, double battery);
void inOrderBST(BSTNode* root);

void run_step_05();
#endif