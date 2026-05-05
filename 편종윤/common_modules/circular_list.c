#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf 등
#include <stdlib.h>               // malloc, free
#include "circular_list.h"        // 원형 연결 리스트 헤더 포함

/* ════════════════════════════════════════════════════════════
   CL_create : 빈 원형 연결 리스트 생성
   ════════════════════════════════════════════════════════════ */
CircularList* CL_create(void) {
    CircularList* CL = (CircularList*)malloc(sizeof(CircularList)); // 헤드 구조체 할당
    CL->head = NULL;   // head 포인터를 NULL로 초기화
    CL->size = 0;      // 요소 수를 0으로 초기화
    return CL;         // 생성된 리스트 반환
}

/* ════════════════════════════════════════════════════════════
   CL_free : 리스트 전체 메모리 해제
   ════════════════════════════════════════════════════════════ */
void CL_free(CircularList* CL) {
    CL_clear(CL);   // 모든 노드 먼저 해제
    free(CL);       // 헤드 구조체 해제
}

/* ════════════════════════════════════════════════════════════
   CL_isEmpty : 리스트가 비어있는지 확인
   ════════════════════════════════════════════════════════════ */
int CL_isEmpty(CircularList* CL) {
    return CL->head == NULL;   // head가 NULL이면 1(true) 반환
}

/* ════════════════════════════════════════════════════════════
   CL_insertFront : 리스트 앞(head)에 새 노드 삽입
   마지막 노드의 link를 새 노드로 업데이트해야 원형 유지
   ════════════════════════════════════════════════════════════ */
void CL_insertFront(CircularList* CL, element val) {
    CL_Node* newNode = (CL_Node*)malloc(sizeof(CL_Node)); // 새 노드 할당
    newNode->data = val;           // 데이터 저장

    if (CL_isEmpty(CL)) {          // 리스트가 비어있으면
        newNode->link = newNode;   // 자기 자신을 가리켜 원형 구성
        CL->head      = newNode;   // head를 새 노드로 설정
    } else {                       // 이미 노드가 있으면
        /* ── 마지막 노드 탐색 (원형 유지를 위해 link 업데이트 필요) ── */
        CL_Node* last = CL->head;            // 마지막 노드 탐색 포인터
        while (last->link != CL->head)       // head로 돌아올 때까지 순회
            last = last->link;               // 다음 노드로 이동
        newNode->link = CL->head;            // 새 노드의 다음을 기존 head로 연결
        last->link    = newNode;             // 마지막 노드의 다음을 새 노드로 연결
        CL->head      = newNode;             // head를 새 노드로 업데이트
    }
    CL->size++;   // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   CL_insertLast : 리스트 맨 끝에 새 노드 삽입
   새 노드의 link는 head를 가리켜 원형 유지
   ════════════════════════════════════════════════════════════ */
void CL_insertLast(CircularList* CL, element val) {
    CL_Node* newNode = (CL_Node*)malloc(sizeof(CL_Node)); // 새 노드 할당
    newNode->data = val;           // 데이터 저장

    if (CL_isEmpty(CL)) {          // 리스트가 비어있으면
        newNode->link = newNode;   // 자기 자신을 가리켜 원형 구성
        CL->head      = newNode;   // head를 새 노드로 설정
    } else {                       // 이미 노드가 있으면
        CL_Node* last = CL->head;            // 마지막 노드 탐색 포인터
        while (last->link != CL->head)       // head로 돌아올 때까지 순회
            last = last->link;               // 다음 노드로 이동
        last->link    = newNode;             // 현재 마지막 노드의 다음을 새 노드로 연결
        newNode->link = CL->head;            // 새 노드의 다음을 head로 연결 (원형 유지)
    }
    CL->size++;   // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   CL_deleteFirst : head(첫 번째) 노드 삭제
   마지막 노드의 link를 새 head로 업데이트
   ════════════════════════════════════════════════════════════ */
void CL_deleteFirst(CircularList* CL) {
    if (CL_isEmpty(CL)) {                    // 비어있으면
        printf("[CL 오류] 빈 리스트!\n");   // 오류 출력
        return;                              // 함수 종료
    }
    if (CL->head->link == CL->head) {        // 노드가 하나뿐이면 (자기 자신을 가리키면)
        free(CL->head);                      // head 노드 해제
        CL->head = NULL;                     // head를 NULL로 초기화
        CL->size--;                          // 요소 수 감소
        return;                              // 함수 종료
    }
    /* ── 노드가 2개 이상인 경우 ── */
    CL_Node* last = CL->head;                // 마지막 노드 탐색 포인터
    while (last->link != CL->head)          // head로 돌아올 때까지 순회
        last = last->link;                  // 다음 노드로 이동

    CL_Node* del = CL->head;               // 삭제할 head 노드 포인터
    last->link   = del->link;             // 마지막 노드의 link를 head의 다음으로 연결
    CL->head     = del->link;             // head를 head의 다음 노드로 업데이트
    free(del);                             // 이전 head 노드 메모리 해제
    CL->size--;                            // 요소 수 감소
}

/* ════════════════════════════════════════════════════════════
   CL_deleteVal : 특정 값(val)의 첫 번째 노드 삭제
   ════════════════════════════════════════════════════════════ */
void CL_deleteVal(CircularList* CL, element val) {
    if (CL_isEmpty(CL)) {                    // 비어있으면
        printf("[CL 오류] 빈 리스트!\n");   // 오류 출력
        return;                              // 함수 종료
    }
    if (CL->head->data == val) {             // head 노드가 대상이면
        CL_deleteFirst(CL);                  // 첫 노드 삭제로 위임
        return;                              // 함수 종료
    }
    CL_Node* prev = CL->head;               // 이전 노드 포인터
    CL_Node* curr = CL->head->link;         // 현재 노드 포인터
    do {                                     // 원형 리스트 순회 (do-while)
        if (curr->data == val) {            // 값 일치하는 노드 발견
            prev->link = curr->link;        // 이전 노드의 다음을 현재의 다음으로 연결
            free(curr);                     // 현재 노드 메모리 해제
            CL->size--;                     // 요소 수 감소
            return;                         // 함수 종료
        }
        prev = curr;                        // 이전 노드를 현재로 이동
        curr = curr->link;                  // 현재 노드를 다음으로 이동
    } while (curr != CL->head);            // head로 돌아오면 순회 종료
    printf("[CL] 값 %d 없음\n", (int)val); // 탐색 실패 안내
}

/* ════════════════════════════════════════════════════════════
   CL_search : val을 가진 첫 번째 노드 포인터 반환
   없으면 NULL 반환
   ════════════════════════════════════════════════════════════ */
CL_Node* CL_search(CircularList* CL, element val) {
    if (CL_isEmpty(CL)) return NULL;   // 비어있으면 NULL 반환
    CL_Node* p = CL->head;             // 순회 포인터를 head로 설정
    do {                               // 원형 리스트 순회 (do-while)
        if (p->data == val)            // 값 일치하면
            return p;                  // 해당 노드 포인터 반환
        p = p->link;                   // 다음 노드로 이동
    } while (p != CL->head);          // head로 돌아오면 종료
    return NULL;                       // 없으면 NULL 반환
}

/* ════════════════════════════════════════════════════════════
   CL_size : 현재 요소 수 반환
   ════════════════════════════════════════════════════════════ */
int CL_size(CircularList* CL) {
    return CL->size;   // size 필드 반환
}

/* ════════════════════════════════════════════════════════════
   CL_print : 리스트 한 바퀴 출력 (head부터 시작, 원형 표시)
   ════════════════════════════════════════════════════════════ */
void CL_print(CircularList* CL) {
    if (CL_isEmpty(CL)) {                      // 비어있으면
        printf("[CircularList] 빈 리스트\n"); // 안내 출력
        return;                                // 함수 종료
    }
    printf("[CircularList] 크기=%d | ", CL->size); // 크기 출력
    printf("head → ");                              // 방향 표시
    CL_Node* p = CL->head;                          // 순회 포인터를 head로 설정
    do {                                            // 원형 리스트 한 바퀴 순회
        printf("%d → ", (int)p->data);              // 각 노드 데이터 출력
        p = p->link;                                // 다음 노드로 이동
    } while (p != CL->head);                        // head로 돌아오면 종료
    printf("(head)\n");                             // 원형 표시 (head로 돌아옴)
}

/* ════════════════════════════════════════════════════════════
   CL_sortAsc : 오름차순 버블 정렬 (data 교환 방식)
   ════════════════════════════════════════════════════════════ */
void CL_sortAsc(CircularList* CL) {
    if (CL->size < 2) return;   // 노드가 2개 미만이면 정렬 불필요
    int swapped;                // 교환 발생 여부 플래그
    do {
        swapped = 0;            // 플래그 초기화
        CL_Node* p = CL->head;  // 순회 포인터를 head로 설정
        do {                    // 한 바퀴 순회
            if (p->data > p->link->data) {  // 현재가 다음보다 크면
                element tmp  = p->data;     // 임시 저장
                p->data      = p->link->data; // 교환
                p->link->data = tmp;        // 교환 완료
                swapped = 1;               // 교환 발생 표시
            }
            p = p->link;                   // 다음 노드로 이동
        } while (p->link != CL->head);    // head 직전 노드까지 순회
    } while (swapped);          // 교환이 발생한 동안 반복
}

/* ════════════════════════════════════════════════════════════
   CL_clear : 전체 노드 해제 (비우기)
   ════════════════════════════════════════════════════════════ */
void CL_clear(CircularList* CL) {
    while (!CL_isEmpty(CL))   // 노드가 남은 동안 반복
        CL_deleteFirst(CL);   // 첫 노드 삭제로 해제
}
