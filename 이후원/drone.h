#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef struct singleLinkedList
{
	char name;
	int x;
	int y;
	struct singleLinkedList* next;
}SLL;

typedef struct singleLinkedListHead
{
	SLL* head;
}SLLH;

typedef struct singleLinkedList_4
{
	char point[11];
	double d;
	double k;
	double batteryCon;
	struct singleLinkedList_4* next;
}SLL4;

SLLH* createSingleLinkedList();
void insertLastNode(SLLH* L, int x, int y, char n);
void bubbleSort(SLLH* L);
void freeLinkedList(SLLH* L);
void pinPointing(SLLH* L);
double distance(SLL* p);
void printDistance(double* d, SLL* t);
void push(char* wp, double item, double k, double bc);
SLL4* pop();
char* navigate(SLL4* t);
int isStackEmpty();
void makeWayPoint();
void arrangeWayPoint();
void makeDistancePerBatteryTable();