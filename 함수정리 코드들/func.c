#include "drone.h"

static SLL4* top = NULL; // 스택 관리 변수

// --- [리스트 및 유틸리티] ---
SLLH* createSingleLinkedList() {
    SLLH* h = (SLLH*)malloc(sizeof(SLLH));
    if (h) h->head = NULL;
    return h;
}

void insertLastNode(SLLH* L, int x, int y, char n)
{
    int count = 1;
    SLL* newNode;
    SLL* temp;
    newNode = (SLL*)malloc(sizeof(SLL));
    newNode->x = x;
    newNode->y = y;
    newNode->name = n;
    newNode->next = NULL;
    if (L->head == NULL) {
        L->head = newNode;
        return;
    }

    temp = L->head;
    while (temp->next != NULL)
    {
        temp = temp->next;
        count++;
    }
    temp->next = newNode;
    count++;
}


void bubbleSort(SLLH* L) {
    if (!L || !L->head) return;
    for (int i = 0; i < MAX - 1; i++) {
        SLL* curr = L->head;
        for (int j = 0; j < MAX - 1 - i; j++) {
            if (curr && curr->next) {
                if ((curr->x * curr->x + curr->y * curr->y) > (curr->next->x * curr->next->x + curr->next->y * curr->next->y)) {
                    int tx = curr->x, ty = curr->y;
                    curr->x = curr->next->x; curr->y = curr->next->y;
                    curr->next->x = tx; curr->next->y = ty;
                }
                curr = curr->next;
            }
        }
    }
}

void pinPointing(SLLH* L) {
    SLL* temp = L->head;
    for (int i = 0; temp != NULL; i++) {
        temp->name = 'A' + i;
        temp = temp->next;
    }
}

void freeLinkedList(SLLH* L) {
    if (!L) return;
    SLL* p = L->head;
    while (p) { SLL* next = p->next; free(p); p = next; }
    free(L);
}

double distance(SLL* p) {
    if (!p || !p->next) return 0;
    return sqrt(pow(p->x - p->next->x, 2) + pow(p->y - p->next->y, 2));
}

void printDistance(double* d, SLL* t) {
    FILE* fpw = fopen("03.txt", "w");
    if (!fpw || !t) return;
    fprintf(stdout, "%10s %10s %10s %11s\n", "구간", "거리(m)", " k", "배터리 소모");
    fprintf(fpw, "%10s %10s %10s %11s\n", "구간", "거리(m)", " k", "배터리 소모");
    SLL* curr = t;
    double td = 0, tb = 0;
    for (int i = 0; i < MAX - 1 && curr->next; i++) {
        char wp[3] = { curr->name, curr->next->name, '\0' };
        double k = (d[i] > 100.0) ? 1.7 : 1.5;
        double consumed = k * d[i];
        fprintf(fpw, "%10s %10.1lf %10.1lf %11.1lf\n", wp, d[i], k, consumed);
        fprintf(stdout, "%10s %10.1lf %10.1lf %11.1lf\n", wp, d[i], k, consumed);
        td += d[i]; tb += consumed;
        curr = curr->next;
    }
    fprintf(fpw, "%10s %10.1lf %10c %11.1lf\n", "TOTAL", td, '-', tb);
    fprintf(stdout, "%10s %10.1lf %10c %11.1lf\n", "TOTAL", td, '-', tb);
    fclose(fpw);
}

// --- [스택 구현] ---
int isStackEmpty() { return top == NULL; }

void push(char* wp, double d, double k, double bc) {
    SLL4* temp = (SLL4*)malloc(sizeof(SLL4));
    strcpy(temp->point, wp); temp->d = d; temp->k = k; temp->batteryCon = bc;
    temp->next = top; top = temp;
}

SLL4* pop() {
    if (isStackEmpty()) return NULL;
    SLL4* temp = top; top = top->next;
    return temp;
}

char* navigate(SLL4* t) {
    char* nav = (char*)malloc(100);
    nav[0] = '\0';
    SLL4* curr = (t == NULL) ? top : t; // 파라미터가 NULL이면 내부 top 사용
    while (curr) {
        strcat(nav, curr->point);
        if (curr->next) strcat(nav, "→");
        curr = curr->next;
    }
    return nav;
}