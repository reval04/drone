#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf 등
#include <stdlib.h>               // malloc, free
#include "single_list.h"          // 단순 연결 리스트 헤더 포함

/* ════════════════════════════════════════════════════════════
   SL_create : 빈 단순 연결 리스트 생성
   ════════════════════════════════════════════════════════════ */
SingleList* SL_create(void) {
    SingleList* L = (SingleList*)malloc(sizeof(SingleList)); // 헤드 구조체 할당
    L->head = NULL;   // head 포인터를 NULL로 초기화
    L->size = 0;      // 요소 수를 0으로 초기화
    return L;         // 생성된 리스트 반환
}

/* ════════════════════════════════════════════════════════════
   SL_free : 리스트 전체 메모리 해제
   ════════════════════════════════════════════════════════════ */
void SL_free(SingleList* L) {
    SL_clear(L);   // 모든 노드 먼저 해제
    free(L);       // 헤드 구조체 해제
}

/* ════════════════════════════════════════════════════════════
   SL_isEmpty : 리스트가 비어있는지 확인
   ════════════════════════════════════════════════════════════ */
int SL_isEmpty(SingleList* L) {
    return L->head == NULL;   // head가 NULL이면 1(true) 반환
}

/* ════════════════════════════════════════════════════════════
   SL_insertFront : 리스트 앞(head)에 새 노드 삽입
   ════════════════════════════════════════════════════════════ */
void SL_insertFront(SingleList* L, element val) {
    SL_Node* newNode = (SL_Node*)malloc(sizeof(SL_Node)); // 새 노드 할당
    newNode->data = val;     // 데이터 저장
    newNode->link = L->head; // 기존 head를 새 노드 다음으로 연결
    L->head       = newNode; // head를 새 노드로 업데이트
    L->size++;               // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   SL_insertLast : 리스트 맨 끝에 새 노드 삽입
   ════════════════════════════════════════════════════════════ */
void SL_insertLast(SingleList* L, element val) {
    SL_Node* newNode = (SL_Node*)malloc(sizeof(SL_Node)); // 새 노드 할당
    newNode->data = val;    // 데이터 저장
    newNode->link = NULL;   // 마지막 노드이므로 link는 NULL

    if (SL_isEmpty(L)) {        // 리스트가 비어있으면
        L->head = newNode;      // 새 노드가 head
    } else {                    // 이미 노드가 있으면
        SL_Node* p = L->head;  // 순회 포인터를 head로 설정
        while (p->link != NULL) // 마지막 노드까지 이동
            p = p->link;        // 다음 노드로 이동
        p->link = newNode;      // 마지막 노드의 다음을 새 노드로 연결
    }
    L->size++;   // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   SL_insertAt : pos 위치에 새 노드 삽입 (0부터 시작)
   ════════════════════════════════════════════════════════════ */
void SL_insertAt(SingleList* L, int pos, element val) {
    if (pos < 0 || pos > L->size) {                      // 범위 벗어난 경우
        printf("[SL 오류] 잘못된 위치: %d\n", pos);     // 오류 출력
        return;                                           // 함수 종료
    }
    if (pos == 0) { SL_insertFront(L, val); return; }   // 앞 삽입으로 위임
    if (pos == L->size) { SL_insertLast(L, val); return; } // 끝 삽입으로 위임

    SL_Node* newNode = (SL_Node*)malloc(sizeof(SL_Node)); // 새 노드 할당
    newNode->data = val;    // 데이터 저장

    SL_Node* prev = L->head;           // 이전 노드 포인터
    for (int i = 0; i < pos - 1; i++) // pos 이전 노드까지 이동
        prev = prev->link;             // 다음 노드로 이동
    newNode->link = prev->link;        // 새 노드의 다음을 이전 노드의 다음으로 연결
    prev->link    = newNode;           // 이전 노드의 다음을 새 노드로 연결
    L->size++;                         // 요소 수 증가
}

/* ════════════════════════════════════════════════════════════
   SL_deleteFirst : head(첫 번째) 노드 삭제
   ════════════════════════════════════════════════════════════ */
void SL_deleteFirst(SingleList* L) {
    if (SL_isEmpty(L)) {                    // 비어있으면
        printf("[SL 오류] 빈 리스트!\n");  // 오류 출력
        return;                             // 함수 종료
    }
    SL_Node* del = L->head;    // 삭제할 head 노드
    L->head      = del->link;  // head를 다음 노드로 이동
    free(del);                  // 이전 head 메모리 해제
    L->size--;                  // 요소 수 감소
}

/* ════════════════════════════════════════════════════════════
   SL_deleteLast : 마지막 노드 삭제
   ════════════════════════════════════════════════════════════ */
void SL_deleteLast(SingleList* L) {
    if (SL_isEmpty(L)) {                    // 비어있으면
        printf("[SL 오류] 빈 리스트!\n");  // 오류 출력
        return;                             // 함수 종료
    }
    if (L->head->link == NULL) {            // 노드가 하나뿐이면
        free(L->head);                      // head 노드 해제
        L->head = NULL;                     // head를 NULL로 초기화
        L->size--;                          // 요소 수 감소
        return;                             // 함수 종료
    }
    SL_Node* prev = L->head;               // 이전 노드 포인터
    while (prev->link->link != NULL)       // 마지막 이전 노드까지 이동
        prev = prev->link;                 // 다음 노드로 이동
    free(prev->link);                      // 마지막 노드 메모리 해제
    prev->link = NULL;                     // 이전 노드의 link를 NULL로
    L->size--;                             // 요소 수 감소
}

/* ════════════════════════════════════════════════════════════
   SL_deleteVal : 특정 값(val)을 가진 첫 번째 노드 삭제
   ════════════════════════════════════════════════════════════ */
void SL_deleteVal(SingleList* L, element val) {
    if (SL_isEmpty(L)) {                    // 비어있으면
        printf("[SL 오류] 빈 리스트!\n");  // 오류 출력
        return;                             // 함수 종료
    }
    if (L->head->data == val) {             // head 노드가 대상이면
        SL_deleteFirst(L);                  // 첫 노드 삭제로 위임
        return;                             // 함수 종료
    }
    SL_Node* prev = L->head;               // 이전 노드 포인터
    while (prev->link != NULL) {           // 끝까지 순회
        if (prev->link->data == val) {     // 다음 노드가 대상이면
            SL_Node* del = prev->link;     // 삭제할 노드 포인터
            prev->link   = del->link;      // 이전 노드의 다음을 그 다음으로 연결
            free(del);                     // 삭제 노드 메모리 해제
            L->size--;                     // 요소 수 감소
            return;                        // 함수 종료
        }
        prev = prev->link;                 // 다음 노드로 이동
    }
    printf("[SL] 값 %d 없음\n", (int)val); // 탐색 실패 안내
}

/* ════════════════════════════════════════════════════════════
   SL_search : val을 가진 첫 번째 노드 포인터 반환
   없으면 NULL 반환
   ════════════════════════════════════════════════════════════ */
SL_Node* SL_search(SingleList* L, element val) {
    SL_Node* p = L->head;       // 탐색 포인터를 head로 설정
    while (p != NULL) {         // 끝까지 순회
        if (p->data == val)     // 값 일치하면
            return p;           // 해당 노드 포인터 반환
        p = p->link;            // 다음 노드로 이동
    }
    return NULL;                // 없으면 NULL 반환
}

/* ════════════════════════════════════════════════════════════
   SL_get : pos 위치 요소 값 반환
   ════════════════════════════════════════════════════════════ */
element SL_get(SingleList* L, int pos) {
    if (pos < 0 || pos >= L->size) {                  // 범위 벗어난 경우
        printf("[SL 오류] 잘못된 위치: %d\n", pos);  // 오류 출력
        return (element)0;                             // 기본값 반환
    }
    SL_Node* p = L->head;           // 순회 포인터를 head로 설정
    for (int i = 0; i < pos; i++)   // pos까지 이동
        p = p->link;                // 다음 노드로 이동
    return p->data;                 // pos 위치 데이터 반환
}

/* ════════════════════════════════════════════════════════════
   SL_size : 현재 요소 수 반환
   ════════════════════════════════════════════════════════════ */
int SL_size(SingleList* L) {
    return L->size;   // size 필드 반환
}

/* ════════════════════════════════════════════════════════════
   SL_print : 리스트 전체 내용 출력 (head → tail 순서)
   ════════════════════════════════════════════════════════════ */
void SL_print(SingleList* L) {
    printf("[SingleList] 크기=%d | ", L->size); // 크기 출력
    printf("head → ");                           // 방향 표시
    SL_Node* p = L->head;                        // 순회 포인터를 head로 설정
    while (p != NULL) {                          // 끝까지 순회
        printf("%d ", (int)p->data);             // 각 노드 데이터 출력
        if (p->link) printf("→ ");              // 다음이 있으면 화살표 출력
        p = p->link;                             // 다음 노드로 이동
    }
    printf("NULL\n");                            // 마지막에 NULL 표시
}

/* ════════════════════════════════════════════════════════════
   SL_sortAsc : 오름차순 버블 정렬 (data 교환 방식)
   ════════════════════════════════════════════════════════════ */
void SL_sortAsc(SingleList* L) {
    if (L->size < 2) return;   // 노드가 2개 미만이면 정렬 불필요
    int swapped;               // 교환 발생 여부 플래그
    do {
        swapped = 0;           // 플래그 초기화
        SL_Node* p = L->head;  // 순회 포인터를 head로 설정
        while (p->link != NULL) {          // 마지막 이전 노드까지 순회
            if (p->data > p->link->data) { // 앞 데이터가 더 크면
                element tmp  = p->data;    // 임시 저장
                p->data      = p->link->data; // 교환
                p->link->data = tmp;       // 교환 완료
                swapped = 1;               // 교환 발생 표시
            }
            p = p->link;   // 다음 노드로 이동
        }
    } while (swapped);     // 교환이 발생한 동안 반복
}

/* ════════════════════════════════════════════════════════════
   SL_reverse : 리스트를 역순으로 재연결
   prev/curr/next 포인터를 이용해 링크 방향 뒤집기
   ════════════════════════════════════════════════════════════ */
void SL_reverse(SingleList* L) {
    SL_Node* prev = NULL;    // 이전 노드 포인터 (초기값 NULL)
    SL_Node* curr = L->head; // 현재 노드 포인터
    SL_Node* next = NULL;    // 다음 노드 포인터
    while (curr != NULL) {   // 끝까지 순회
        next       = curr->link;  // 다음 노드 저장
        curr->link = prev;        // 현재 노드의 link를 이전 노드로 역전
        prev       = curr;        // 이전 노드를 현재 노드로 이동
        curr       = next;        // 현재 노드를 다음 노드로 이동
    }
    L->head = prev;          // head를 새로운 첫 노드(기존 마지막)로 업데이트
}

/* ════════════════════════════════════════════════════════════
   SL_clear : 전체 노드 해제 (비우기)
   ════════════════════════════════════════════════════════════ */
void SL_clear(SingleList* L) {
    while (!SL_isEmpty(L))   // 노드가 남은 동안 반복
        SL_deleteFirst(L);   // 첫 노드 삭제로 해제
}
