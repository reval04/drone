#define _CRT_SECURE_NO_WARNINGS   // MSVC에서 scanf/fopen 등 보안 경고 억제
#include <stdio.h>                // printf, FILE, fopen, fprintf, fscanf 등
#include <stdlib.h>               // malloc, free 동적 메모리 함수
#include <math.h>                 // sqrt() 함수 (거리 계산용)
#include "droneList.h"            // 이 프로젝트의 자료구조 헤더 포함

/* ════════════════════════════════════════════════════════════
   createLinkedList : 빈 연결 리스트를 동적으로 생성하여 반환
   ════════════════════════════════════════════════════════════ */
linkedList_h* createLinkedList(void) {
    linkedList_h* L;                                    // 헤드 구조체 포인터 선언
    L = (linkedList_h*)malloc(sizeof(linkedList_h));    // 헤드 구조체 크기만큼 힙 메모리 할당
    L->head = NULL;                                     // 아직 노드가 없으므로 head를 NULL로 초기화
    L->size = 0;                                        // 노드 개수 0으로 초기화
    return L;                                           // 생성된 리스트 포인터 반환
}

/* ════════════════════════════════════════════════════════════
   freeLinkedList : 리스트의 모든 노드와 헤드 구조체 메모리 해제
   ════════════════════════════════════════════════════════════ */
void freeLinkedList(linkedList_h* L) {
    listNode* p;                      // 해제할 노드를 임시로 보관할 포인터
    while (L->head != NULL) {         // 리스트에 노드가 남아있는 동안 반복
        p       = L->head;            // 현재 head 노드를 p에 저장
        L->head = L->head->link;      // head를 다음 노드로 이동
        free(p);                      // 이전 head 노드 메모리 반환
    }
    free(L);                          // 헤드 구조체 자체 메모리 반환
}

/* ════════════════════════════════════════════════════════════
   printList : 리스트의 모든 경유지를 화면에 출력
   ════════════════════════════════════════════════════════════ */
void printList(linkedList_h* L) {
    listNode* p = L->head;                                  // 순회 포인터를 첫 노드에 설정
    printf("경유지 목록 (총 %d개):\n", L->size);            // 전체 노드 개수 출력
    while (p != NULL) {                                     // 노드가 없을 때까지 순회
        printf("  [ID:%3d]  위치:(%4d, %4d)  고도:%4dm\n", // 각 필드를 정렬하여 출력
               p->id, p->x, p->y, p->altitude);
        p = p->link;                                        // 다음 노드로 이동
    }
}

/* ════════════════════════════════════════════════════════════
   insertLastNode : 리스트 맨 끝에 새 경유지 노드 삽입
   ════════════════════════════════════════════════════════════ */
void insertLastNode(linkedList_h* L, int id, int x, int y, int alt) {
    listNode* newNode;                                 // 새 노드 포인터 선언
    newNode           = (listNode*)malloc(sizeof(listNode)); // 새 노드 메모리 할당
    newNode->id       = id;                            // 경유지 ID 저장
    newNode->x        = x;                             // X 좌표 저장
    newNode->y        = y;                             // Y 좌표 저장
    newNode->altitude = alt;                           // 고도 저장
    newNode->link     = NULL;                          // 마지막 노드이므로 link는 NULL

    if (L->head == NULL) {                             // 리스트가 비어있으면
        L->head = newNode;                             // 새 노드가 곧 첫 번째 노드
    } else {                                           // 리스트에 노드가 이미 있으면
        listNode* temp = L->head;                      // 임시 포인터를 head에 설정
        while (temp->link != NULL) temp = temp->link;  // 마지막 노드까지 이동
        temp->link = newNode;                          // 마지막 노드의 link에 새 노드 연결
    }
    L->size++;                                         // 노드 수 1 증가
}

/* ════════════════════════════════════════════════════════════
   deleteNodeById : 지정한 ID의 경유지 노드 삭제
   반환값: 삭제 성공 1, 해당 ID 없음 0
   ════════════════════════════════════════════════════════════ */
int deleteNodeById(linkedList_h* L, int id) {
    if (L->head == NULL) return 0;      // 리스트가 비어있으면 삭제 불가, 0 반환

    listNode* p   = L->head;            // 탐색용 포인터를 head에 설정
    listNode* pre = NULL;               // p의 이전 노드를 가리킬 포인터 (초기 NULL)

    while (p != NULL) {                 // 리스트 끝까지 순회
        if (p->id == id) {              // 찾는 ID와 일치하는 노드 발견
            if (pre == NULL)            // 삭제 노드가 첫 번째 노드인 경우
                L->head = p->link;      // head를 다음 노드로 변경
            else                        // 삭제 노드가 중간/마지막 노드인 경우
                pre->link = p->link;    // 이전 노드의 link를 삭제 노드의 다음으로 우회
            free(p);                    // 삭제 노드 메모리 반환
            L->size--;                  // 노드 수 1 감소
            return 1;                   // 삭제 성공, 1 반환
        }
        pre = p;                        // 이전 노드 포인터 갱신
        p   = p->link;                  // 다음 노드로 이동
    }
    return 0;                           // 해당 ID의 노드를 찾지 못함, 0 반환
}

/* ════════════════════════════════════════════════════════════
   searchNodeById : 지정한 ID의 경유지 노드 탐색
   반환값: 노드 포인터 (없으면 NULL)
   ════════════════════════════════════════════════════════════ */
listNode* searchNodeById(linkedList_h* L, int id) {
    listNode* p = L->head;      // 탐색 포인터를 head에 설정
    while (p != NULL) {         // 리스트 끝까지 순회
        if (p->id == id)        // ID가 일치하는 노드 발견
            return p;           // 해당 노드 포인터 반환
        p = p->link;            // 다음 노드로 이동
    }
    return NULL;                // 찾지 못한 경우 NULL 반환
}

/* ════════════════════════════════════════════════════════════
   saveToFile : 리스트 내용을 텍스트 파일로 저장
   파일 형식 → 첫 줄: 노드 개수 / 이후 줄: id x y altitude
   ════════════════════════════════════════════════════════════ */
void saveToFile(linkedList_h* L, const char* filename) {
    FILE* fp = fopen(filename, "w");            // 쓰기 모드로 파일 열기
    if (fp == NULL) {                           // 파일 열기 실패 처리
        printf("파일 열기 실패: %s\n", filename); // 오류 메시지 출력
        return;                                 // 함수 종료
    }
    fprintf(fp, "%d\n", L->size);              // 첫 줄에 총 노드 수 기록
    listNode* p = L->head;                     // 순회 포인터를 head로 설정
    while (p != NULL) {                        // 모든 노드를 순서대로 파일에 기록
        fprintf(fp, "%d %d %d %d\n",           // id, x, y, altitude를 한 줄에 기록
                p->id, p->x, p->y, p->altitude);
        p = p->link;                           // 다음 노드로 이동
    }
    fclose(fp);                                // 파일 닫기 (버퍼 flush 포함)
    printf("저장 완료: %s (%d개)\n", filename, L->size); // 완료 메시지 출력
}

/* ════════════════════════════════════════════════════════════
   loadFromFile : 텍스트 파일에서 경유지 데이터를 읽어 리스트에 추가
   ════════════════════════════════════════════════════════════ */
void loadFromFile(linkedList_h* L, const char* filename) {
    FILE* fp = fopen(filename, "r");             // 읽기 모드로 파일 열기
    if (fp == NULL) {                            // 파일 열기 실패 처리
        printf("파일 열기 실패: %s\n", filename);  // 오류 메시지 출력
        return;                                  // 함수 종료
    }
    int count, id, x, y, alt;                   // 임시 변수 선언
    fscanf(fp, "%d", &count);                   // 첫 줄에서 노드 개수 읽기
    while (fscanf(fp, "%d %d %d %d",            // 파일에서 4개 값 읽기
                  &id, &x, &y, &alt) == 4) {    // 4개 모두 읽었을 때만 처리
        insertLastNode(L, id, x, y, alt);        // 읽은 데이터로 노드 삽입
    }
    fclose(fp);                                  // 파일 닫기
    printf("로드 완료: %s (%d개)\n", filename, L->size); // 완료 메시지 출력
}

/* ════════════════════════════════════════════════════════════
   calcTotalDistance : 경유지들 사이 3D 유클리드 거리의 합 계산
   거리 공식: sqrt((dx)^2 + (dy)^2 + (dz)^2)
   ════════════════════════════════════════════════════════════ */
double calcTotalDistance(linkedList_h* L) {
    if (L->head == NULL || L->head->link == NULL) // 노드가 0개 또는 1개면
        return 0.0;                               // 이동 거리 0 반환
    double total = 0.0;                           // 누적 거리 초기화
    listNode* p  = L->head;                       // 순회 포인터를 첫 노드로 설정
    while (p->link != NULL) {                     // 마지막 노드 전까지 반복
        double dx = (double)(p->link->x        - p->x);        // x축 거리 차이
        double dy = (double)(p->link->y        - p->y);        // y축 거리 차이
        double dz = (double)(p->link->altitude - p->altitude); // z축(고도) 거리 차이
        total += sqrt(dx*dx + dy*dy + dz*dz);    // 3D 거리를 누적 합산
        p = p->link;                             // 다음 노드 쌍으로 이동
    }
    return total;                                // 최종 합산 거리 반환
}
