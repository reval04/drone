#include"kys.h"
#include "Drone.h"



void addCoordnate(FILE* f) {
    if (f == NULL)
        return;

    char name[3];
    for (int i = 0; i < MAX; i++) {
        int x = rand() % 500;
        int y = rand() % 500;

        fprintf(stdout, "%(d,%d)\t " , x, y);
        fprintf(f, "%s %d %d\n", x, y);
    }

    fclose(f);
}

// 두 점 사이의 거리를 계산하는 함수
double getDistance(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;

    return sqrt((double)(dx * dx + dy * dy));
}

Node* extract_node(Node* prev_node) { //중간 노드 꺼내기
    if (prev_node == NULL || prev_node->link == NULL) return NULL;

    Node* target = prev_node->link;   
    prev_node->link = target->link;   
    target->link = NULL;             
    return target;
}



Node* getNearestPoint(int curX, int curY, Node* head) {
    if (head->link == NULL) return NULL;

    Node* prev = head;
    Node* tmp = head->link;

    Node* minPrev = head;
    double minDistance = 999999.0;

    while (tmp != NULL) {
        double d = getDistance(curX, curY, tmp->x, tmp->y);
        if (d < minDistance) {
            minDistance = d;
            minPrev = prev;
        }
        prev = tmp;
        tmp = tmp->link;
    }

    // 2. 찾은 노드를 리스트에서 추출해서 반환
 return extract_node(minPrev);
}


void relocation(Drone* drone, Node* waitingList) {
    int curX = 0, curY = 0; // 드론의 시작 위치 (원점)

    Node* resultHead = (Node*)calloc(1, sizeof(Node));
    Node* resultTail = resultHead;


    while (waitingList->link != NULL) {
        Node* next = getNearestPoint(curX, curY, waitingList);

        while (waitingList->link != NULL) {
            Node* next = getNearestNode(curX, curY, waitingList);
            if (next == NULL) break;
            resultTail->link = next;
            resultTail = next;
            curX = next -> x;
            curY = next -> y;

        }
        resultTail->link = NULL;
        drone->head = resultHead->link;
        free(resultHead);
 
}
