#include "drone.h"

static SLL4* top = NULL;

SLLH* createSingleLinkedList() {
    SLLH* h = (SLLH*)malloc(sizeof(SLLH));
    if (h) h->head = NULL;
    return h;
}

void insertLastNode(SLLH* L, int x, int y, char n) {
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
    while (temp->next != NULL) {
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
    FILE* fpw = fopen("03.txt", "a+");
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
    SLL4* curr = (t == NULL) ? top : t;
    while (curr) {
        strcat(nav, curr->point);
        if (curr->next) strcat(nav, "→");
        curr = curr->next;
    }
    return nav;
}

void printIndex(FILE* p) {
    char* menu[] = { "구간", "거리(m)", " k", "배터리 소모", "누적 소모량", "상태", "이벤트", "안정회귀 경로" };
    fprintf(p, "%10s %10s %10s %11s %11s %10s %10s %11s\n",
        *(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3), *(menu + 4), *(menu + 5), *(menu + 6), *(menu + 7));
    printf("%10s %10s %10s %11s %11s %10s %10s %11s\n",
        *(menu + 0), *(menu + 1), *(menu + 2), *(menu + 3), *(menu + 4), *(menu + 5), *(menu + 6), *(menu + 7));
}




//----------------6장 큐잉이론 추가---------------------
void initQueue(Queue* q) { q->front = q->rear = 0; }
int isQueueEmpty(Queue* q) { return q->front == q->rear; }
int isQueueFull(Queue* q) { return (q->rear + 1) % MAX_QUEUE == q->front; }

void enqueue(Queue* q, Drone d) {
    if (isQueueFull(q)) {
        printf("[경고] 정비소 대기열이 꽉 찼습니다!\n");
        return;
    }
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->data[q->rear] = d;
    printf(">> [큐 진입] 드론 %d호기 정비소 대기열 등록 완료\n", d.droneID);
}

Drone dequeue(Queue* q) {
    Drone empty = { 0, 0 };
    if (isQueueEmpty(q)) return empty;
    q->front = (q->front + 1) % MAX_QUEUE;
    printf("<< [큐 이탈] 드론 %d호기 충전 완료 및 재출격 대기\n", q->data[q->front].droneID);
    return q->data[q->front];
}

double checkQueueingTheory(double lambda, double mu) {
    double rho = lambda / mu;
    printf("\n[큐잉이론 분석] 도착률(λ)=%.1lf, 처리율(μ)=%.1lf\n", lambda, mu);
    printf("시스템 부하율(ρ) = %.2lf\n", rho);

    if (rho < 1.0) {
        printf("-> 판단: 충전소 시스템이 안정적입니다. (정상 가동 가능)\n");
    }
    else if (rho < 1.8) {
        printf("-> 판단: [경고] 시스템 과부하 진입! 대기열이 밀리기 시작합니다.\n");
    }
    else {
        printf("-> 판단: [위험] 시스템 치명적 과부하! 정비 공간이 부족합니다.\n");
    }
    return rho;
}



//----------------7장 트리 추가---------------------
TreeNode* createTreeNode(char name) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->name = name;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void preOrder(TreeNode* root) {
    if (root) {
        printf("%c지점 -> ", root->name);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void postOrder(TreeNode* root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%c지점 이상무 -> ", root->name);
    }
}

BSTNode* insertBST(BSTNode* root, char* section, double battery) {
    if (root == NULL) {
        BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
        strcpy(newNode->section, section);
        newNode->batteryConsumed = battery;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (strcmp(root->section, section) == 0) {
        root->batteryConsumed = battery; // 최신 데이터 반영
        return root;
    }

    // 배터리 소모량이 적으면 왼쪽, 많으면 오른쪽
    if (battery < root->batteryConsumed)
        root->left = insertBST(root->left, section, battery);
    else
        root->right = insertBST(root->right, section, battery);
    return root;
}

void inOrderBST(BSTNode* root) {
    if (root) {
        inOrderBST(root->left);
        printf("   - 구간 %s : 소모량 %.1lf\n", root->section, root->batteryConsumed);
        inOrderBST(root->right);
    }
}