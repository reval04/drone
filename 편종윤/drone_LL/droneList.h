#pragma once          // 이 헤더 파일이 한 번만 포함되도록 중복 방지 선언
#include <stdio.h>    // 표준 입출력 함수(printf, FILE 등) 사용을 위한 헤더

/* ──────────────────────────────────────────────────────────
   드론 경유지 노드 구조체
   연결 리스트의 각 노드가 드론 경유지 하나를 표현한다
   ────────────────────────────────────────────────────────── */
typedef struct ListNode {
    int id;                  // 경유지 고유 번호 (1번부터 순서대로)
    int x;                   // 경유지 X 좌표 (단위: m)
    int y;                   // 경유지 Y 좌표 (단위: m)
    int altitude;            // 경유지 고도 (단위: m)
    struct ListNode* link;   // 다음 노드를 가리키는 포인터
} listNode;                  // 구조체 별칭: listNode

/* ──────────────────────────────────────────────────────────
   연결 리스트 헤드 구조체
   리스트 전체를 대표하는 관리 구조체
   ────────────────────────────────────────────────────────── */
typedef struct {
    listNode* head;   // 리스트의 첫 번째 노드를 가리키는 포인터
    int size;         // 현재 리스트에 들어있는 노드(경유지) 수
} linkedList_h;       // 구조체 별칭: linkedList_h

/* ── 함수 원형 선언 ── */
linkedList_h* createLinkedList(void);                            // 빈 연결 리스트 생성
void          freeLinkedList(linkedList_h* L);                   // 연결 리스트 전체 메모리 해제
void          printList(linkedList_h* L);                        // 경유지 목록 화면 출력
void          insertLastNode(linkedList_h* L,                    // 리스트 맨 끝에 경유지 삽입
                             int id, int x, int y, int alt);
int           deleteNodeById(linkedList_h* L, int id);           // 지정 ID 경유지 삭제 (성공:1, 실패:0)
listNode*     searchNodeById(linkedList_h* L, int id);           // 지정 ID 경유지 탐색 후 포인터 반환
void          saveToFile(linkedList_h* L, const char* filename); // 리스트 내용을 파일에 저장
void          loadFromFile(linkedList_h* L, const char* filename);// 파일에서 경유지 데이터를 읽어 리스트 구성
double        calcTotalDistance(linkedList_h* L);                // 모든 경유지 간 3D 거리 합산
