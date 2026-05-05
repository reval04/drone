#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf 등
#include <stdlib.h>               // malloc, free
#include "double_list.h"          // 이중 연결 리스트 헤더 포함

/* ════════════════════════════════════════════════════════════
   DL_create : 빈 이중 연결 리스트 생성
   ════════════════════════════════════════════════════════════ */
DoubleList* DL_create(void) {
    DoubleList* L = (DoubleList*)malloc(sizeof(DoubleList)); // 헤드 구조체 할당
    L->head = NULL;   // head 포인터를 NULL로 초기화
    L->tail = NULL;   // tail 포인터를 NULL로 초기화
    L->size = 0;      // 요소 수를 0으로 초기화
    return L;         // 생성된 리스트 반환
}

/* ════════════════════════════════════════════════════════════
   DL_free : 리스트 전체 메모리 해제
   ════════════════════════════════════════════════════════════ */
void DL_free(DoubleList* L) {
    DL_clear(L);   // 모든 노드 먼저 해제
    free(L);       // 헤드 구조체 해제
}

/* ════════════════════════════════════════════════════════════
   DL_isEmpty : 리스트가 비어있는지 확인
   ════════════════════════════════════════════════════════════ */
int DL_isEmpty(DoubleList* L) {
    return L->head == NULL;   // head가 NULL이면 1(true) 반환
}

/* ════════════════════════════════════════════════════════════
   DL_insertFront : 리스트 앞(head)에 새 노드 삽입
   ════════════════════════════════════════════════════════════ */
void DL_insertFront(DoubleList* L, element val) {
    DL_Node* newNode = (DL_Node*)malloc(sizeof(DL_Node)); // 새 노드 할당
    newNode->data  = val;     // 데이터 저장
    newNode->llink = NULL;    // 첫 노드이므로 이전 포인터는 NULL
    newNode->rlink = L->head; // 기존 head를 새 노드의 다음으로 연결

    if (L->head != NULL)      // 기존 노드가 있으면
        L->head->llink = newNode; // 기존 head의 이전을 새 노드로 연결
    else                      // 리스트가 비어있으면
        L->tail = newNode;    // tail도 새 노드로 설정

    L->head = newNode;        // head를 새 노드로 업데이트
    L->size++;                // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   DL_insertLast : 리스트 맨 끝(tail)에 새 노드 삽입
   ════════════════════════════════════════════════════════════ */
void DL_insertLast(DoubleList* L, element val) {
    DL_Node* newNode = (DL_Node*)malloc(sizeof(DL_Node)); // 새 노드 할당
    newNode->data  = val;     // 데이터 저장
    newNode->rlink = NULL;    // 마지막 노드이므로 다음 포인터는 NULL
    newNode->llink = L->tail; // 기존 tail을 새 노드의 이전으로 연결

    if (L->tail != NULL)      // 기존 노드가 있으면
        L->tail->rlink = newNode; // 기존 tail의 다음을 새 노드로 연결
    else                      // 리스트가 비어있으면
        L->head = newNode;    // head도 새 노드로 설정

    L->tail = newNode;        // tail을 새 노드로 업데이트
    L->size++;                // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   DL_insertAfter : 노드 p 바로 뒤에 새 노드 삽입
   ════════════════════════════════════════════════════════════ */
void DL_insertAfter(DoubleList* L, DL_Node* p, element val) {
    if (p == NULL) { DL_insertLast(L, val); return; } // p가 NULL이면 끝에 삽입
    if (p == L->tail) { DL_insertLast(L, val); return; } // 마지막 노드면 끝 삽입

    DL_Node* newNode = (DL_Node*)malloc(sizeof(DL_Node)); // 새 노드 할당
    newNode->data  = val;       // 데이터 저장
    newNode->llink = p;         // 새 노드의 이전을 p로 연결
    newNode->rlink = p->rlink;  // 새 노드의 다음을 p의 다음으로 연결

    if (p->rlink != NULL)               // p의 다음이 있으면
        p->rlink->llink = newNode;      // 그 노드의 이전을 새 노드로 연결
    p->rlink = newNode;                 // p의 다음을 새 노드로 연결
    L->size++;                          // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   DL_deleteNode : 노드 p를 직접 삭제 (포인터를 알고 있을 때 O(1))
   이중 연결 리스트의 앞뒤 포인터를 모두 수정
   ════════════════════════════════════════════════════════════ */
void DL_deleteNode(DoubleList* L, DL_Node* p) {
    if (p == NULL) return;               // 잘못된 포인터이면 종료

    if (p->llink != NULL)                // 이전 노드가 있으면
        p->llink->rlink = p->rlink;      // 이전 노드의 다음을 p의 다음으로 연결
    else                                 // p가 head인 경우
        L->head = p->rlink;              // head를 p의 다음으로 업데이트

    if (p->rlink != NULL)                // 다음 노드가 있으면
        p->rlink->llink = p->llink;      // 다음 노드의 이전을 p의 이전으로 연결
    else                                 // p가 tail인 경우
        L->tail = p->llink;              // tail을 p의 이전으로 업데이트

    free(p);     // p 노드 메모리 해제
    L->size--;   // 요소 수 감소
}

/* ════════════════════════════════════════════════════════════
   DL_deleteFirst : head 노드 삭제
   ════════════════════════════════════════════════════════════ */
void DL_deleteFirst(DoubleList* L) {
    if (DL_isEmpty(L)) { printf("[DL 오류] 빈 리스트!\n"); return; } // 비어있으면 종료
    DL_deleteNode(L, L->head);   // head 노드를 직접 삭제
}

/* ════════════════════════════════════════════════════════════
   DL_deleteLast : tail 노드 삭제
   ════════════════════════════════════════════════════════════ */
void DL_deleteLast(DoubleList* L) {
    if (DL_isEmpty(L)) { printf("[DL 오류] 빈 리스트!\n"); return; } // 비어있으면 종료
    DL_deleteNode(L, L->tail);   // tail 노드를 직접 삭제
}

/* ════════════════════════════════════════════════════════════
   DL_deleteVal : 특정 값(val)의 첫 번째 노드 삭제
   ════════════════════════════════════════════════════════════ */
void DL_deleteVal(DoubleList* L, element val) {
    DL_Node* p = DL_search(L, val);       // 값으로 노드 탐색
    if (p == NULL) {                      // 없으면
        printf("[DL] 값 %d 없음\n", (int)val); // 안내 출력
        return;                           // 함수 종료
    }
    DL_deleteNode(L, p);   // 찾은 노드 직접 삭제
}

/* ════════════════════════════════════════════════════════════
   DL_search : val을 가진 첫 번째 노드 포인터 반환
   없으면 NULL 반환
   ════════════════════════════════════════════════════════════ */
DL_Node* DL_search(DoubleList* L, element val) {
    DL_Node* p = L->head;       // 탐색 포인터를 head로 설정
    while (p != NULL) {         // 끝까지 순회
        if (p->data == val)     // 값 일치하면
            return p;           // 해당 노드 포인터 반환
        p = p->rlink;           // 다음 노드로 이동
    }
    return NULL;                // 없으면 NULL 반환
}

/* ════════════════════════════════════════════════════════════
   DL_size : 현재 요소 수 반환
   ════════════════════════════════════════════════════════════ */
int DL_size(DoubleList* L) {
    return L->size;   // size 필드 반환
}

/* ════════════════════════════════════════════════════════════
   DL_printFwd : 정방향 출력 (head → tail)
   ════════════════════════════════════════════════════════════ */
void DL_printFwd(DoubleList* L) {
    printf("[DoubleList 정방향] 크기=%d | ", L->size); // 크기 출력
    printf("head → ");                                  // 방향 표시
    DL_Node* p = L->head;                               // 순회 포인터를 head로 설정
    while (p != NULL) {                                 // 끝까지 순회
        printf("%d ", (int)p->data);                    // 각 노드 데이터 출력
        if (p->rlink) printf("⇄ ");                    // 다음이 있으면 화살표
        p = p->rlink;                                   // 다음 노드로 이동
    }
    printf("NULL\n");                                   // 마지막에 NULL 표시
}

/* ════════════════════════════════════════════════════════════
   DL_printBwd : 역방향 출력 (tail → head)
   ════════════════════════════════════════════════════════════ */
void DL_printBwd(DoubleList* L) {
    printf("[DoubleList 역방향] 크기=%d | ", L->size); // 크기 출력
    printf("tail → ");                                  // 방향 표시
    DL_Node* p = L->tail;                               // 순회 포인터를 tail로 설정
    while (p != NULL) {                                 // 끝까지 역방향 순회
        printf("%d ", (int)p->data);                    // 각 노드 데이터 출력
        if (p->llink) printf("⇄ ");                    // 이전이 있으면 화살표
        p = p->llink;                                   // 이전 노드로 이동
    }
    printf("NULL\n");                                   // 마지막에 NULL 표시
}

/* ════════════════════════════════════════════════════════════
   DL_sortAsc : 오름차순 버블 정렬 (data 교환 방식)
   ════════════════════════════════════════════════════════════ */
void DL_sortAsc(DoubleList* L) {
    if (L->size < 2) return;   // 노드가 2개 미만이면 정렬 불필요
    int swapped;               // 교환 발생 여부 플래그
    do {
        swapped = 0;           // 플래그 초기화
        DL_Node* p = L->head;  // 순회 포인터를 head로 설정
        while (p->rlink != NULL) {           // 마지막 이전 노드까지 순회
            if (p->data > p->rlink->data) {  // 앞 데이터가 더 크면
                element tmp    = p->data;    // 임시 저장
                p->data        = p->rlink->data; // 교환
                p->rlink->data = tmp;        // 교환 완료
                swapped = 1;                 // 교환 발생 표시
            }
            p = p->rlink;   // 다음 노드로 이동
        }
    } while (swapped);   // 교환이 발생한 동안 반복
}

/* ════════════════════════════════════════════════════════════
   DL_clear : 전체 노드 해제 (비우기)
   ════════════════════════════════════════════════════════════ */
void DL_clear(DoubleList* L) {
    while (!DL_isEmpty(L))   // 노드가 남은 동안 반복
        DL_deleteFirst(L);   // 첫 노드 삭제로 해제
}
