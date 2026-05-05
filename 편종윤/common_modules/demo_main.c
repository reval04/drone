/* ════════════════════════════════════════════════════════════
   demo_main.c  —  common_modules 전체 사용 예제
   ─────────────────────────────────────────────────────────
   각 모듈의 주요 함수를 실제로 호출해보는 데모 파일.
   새 프로젝트를 시작할 때 이 파일을 참고하면
   각 자료구조 모듈을 어떻게 쓰는지 바로 확인할 수 있다.
   ════════════════════════════════════════════════════════════ */

#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include "common.h"               // 모든 모듈 한 번에 포함

/* ════════════════════════════════════════════════════════════
   demo_fileio : 파일 입출력 유틸리티 데모
   ════════════════════════════════════════════════════════════ */
void demo_fileio(void) {
    printf("\n[=== 파일 I/O 유틸리티 데모 ===]\n");

    /* ── 정수 배열 파일 쓰기 ── */
    int arr[] = {10, 20, 30, 40, 50};   // 테스트용 정수 배열
    writeIntArray("demo_numbers.txt", arr, 5); // 파일에 배열 저장

    /* ── 파일 존재 확인 ── */
    printf("demo_numbers.txt 존재 여부: %s\n",   // 파일 존재 확인
           fileExists("demo_numbers.txt") ? "있음" : "없음");

    /* ── 파일 크기 및 줄 수 확인 ── */
    printf("파일 크기: %ld바이트\n", fileSize("demo_numbers.txt")); // 파일 크기
    printf("파일 줄 수: %d줄\n", countLines("demo_numbers.txt"));   // 줄 수

    /* ── 정수 배열 파일 읽기 ── */
    int readArr[10];                                 // 읽기 버퍼 배열
    int cnt = readIntArray("demo_numbers.txt", readArr, 10); // 파일에서 읽기
    printf("읽어온 값 (%d개): ", cnt);               // 읽은 개수 출력
    for (int i = 0; i < cnt; i++)                   // 읽어온 값 출력
        printf("%d ", readArr[i]);                   // 각 값 출력
    printf("\n");

    /* ── 화면+파일 동시 출력 ── */
    FILE* rp = fopen("demo_report.txt", "w");        // 보고서 파일 열기
    printHeader(rp, "데모 보고서");                  // 헤더 출력 (화면+파일)
    fprintBoth(rp, "항목: %d개, 합계: %d\n",         // 화면+파일 동시 출력
               cnt, 10+20+30+40+50);
    printDivider(rp, 40);                            // 구분선 출력
    printFooter(rp);                                 // 하단 출력
    if (rp) fclose(rp);                              // 파일 닫기
}

/* ════════════════════════════════════════════════════════════
   demo_array_stack : 배열 스택 데모
   ════════════════════════════════════════════════════════════ */
void demo_array_stack(void) {
    printf("\n[=== 배열 스택 (ArrayStack) 데모 ===]\n");

    ArrayStack s;          // 배열 스택 선언
    AS_init(&s);           // 스택 초기화

    /* ── push 테스트 ── */
    printf("push: 10, 20, 30, 40, 50\n"); // 삽입할 값 안내
    AS_push(&s, 10);   // 10 삽입
    AS_push(&s, 20);   // 20 삽입
    AS_push(&s, 30);   // 30 삽입
    AS_push(&s, 40);   // 40 삽입
    AS_push(&s, 50);   // 50 삽입
    AS_print(&s);      // 스택 출력

    /* ── peek / pop 테스트 ── */
    printf("peek: %d\n", (int)AS_peek(&s)); // top 확인 (제거 없음)
    printf("pop: %d\n",  (int)AS_pop(&s));  // top 꺼내기
    printf("pop: %d\n",  (int)AS_pop(&s));  // top 꺼내기
    AS_print(&s);   // pop 후 스택 출력

    printf("크기: %d\n", AS_size(&s)); // 현재 크기 출력
    AS_clear(&s);                       // 스택 비우기
    printf("clear 후 isEmpty: %d\n", AS_isEmpty(&s)); // 비었는지 확인
}

/* ════════════════════════════════════════════════════════════
   demo_linked_stack : 연결 스택 데모
   ════════════════════════════════════════════════════════════ */
void demo_linked_stack(void) {
    printf("\n[=== 연결 스택 (LinkedStack) 데모 ===]\n");

    LinkedStack* s = LS_create();   // 연결 스택 생성

    printf("push: 100, 200, 300\n"); // 삽입할 값 안내
    LS_push(s, 100);   // 100 삽입
    LS_push(s, 200);   // 200 삽입
    LS_push(s, 300);   // 300 삽입
    LS_print(s);       // 스택 출력

    printf("peek: %d\n", (int)LS_peek(s)); // top 확인 (제거 없음)
    printf("pop: %d\n",  (int)LS_pop(s));  // top 꺼내기
    LS_print(s);        // pop 후 출력

    LS_free(s);         // 스택 전체 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   demo_array_queue : 배열 원형 큐 데모
   ════════════════════════════════════════════════════════════ */
void demo_array_queue(void) {
    printf("\n[=== 배열 원형 큐 (ArrayQueue) 데모 ===]\n");

    ArrayQueue q;       // 배열 큐 선언
    AQ_init(&q);        // 큐 초기화

    printf("enqueue: 1, 2, 3, 4, 5\n"); // 삽입할 값 안내
    AQ_enqueue(&q, 1);   // 1 삽입
    AQ_enqueue(&q, 2);   // 2 삽입
    AQ_enqueue(&q, 3);   // 3 삽입
    AQ_enqueue(&q, 4);   // 4 삽입
    AQ_enqueue(&q, 5);   // 5 삽입
    AQ_print(&q);        // 큐 출력

    printf("front: %d\n",   (int)AQ_front(&q));   // front 조회
    printf("dequeue: %d\n", (int)AQ_dequeue(&q)); // front 꺼내기
    printf("dequeue: %d\n", (int)AQ_dequeue(&q)); // front 꺼내기
    AQ_print(&q);   // dequeue 후 출력

    printf("크기: %d\n", AQ_size(&q)); // 현재 크기
}

/* ════════════════════════════════════════════════════════════
   demo_linked_queue : 연결 큐 데모
   ════════════════════════════════════════════════════════════ */
void demo_linked_queue(void) {
    printf("\n[=== 연결 큐 (LinkedQueue) 데모 ===]\n");

    LinkedQueue* q = LQ_create();   // 연결 큐 생성

    printf("enqueue: 10, 20, 30\n"); // 삽입할 값 안내
    LQ_enqueue(q, 10);   // 10 삽입
    LQ_enqueue(q, 20);   // 20 삽입
    LQ_enqueue(q, 30);   // 30 삽입
    LQ_print(q);         // 큐 출력

    printf("dequeue: %d\n", (int)LQ_dequeue(q)); // front 꺼내기
    LQ_print(q);     // dequeue 후 출력

    LQ_free(q);      // 큐 전체 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   demo_array_list : 배열 순차 리스트 데모
   ════════════════════════════════════════════════════════════ */
void demo_array_list(void) {
    printf("\n[=== 배열 순차 리스트 (ArrayList) 데모 ===]\n");

    ArrayList L;       // 배열 리스트 선언
    AL_init(&L);       // 리스트 초기화

    printf("insertLast: 50, 30, 10, 40, 20\n"); // 삽입할 값 안내
    AL_insertLast(&L, 50);   // 50 맨 끝 삽입
    AL_insertLast(&L, 30);   // 30 맨 끝 삽입
    AL_insertLast(&L, 10);   // 10 맨 끝 삽입
    AL_insertLast(&L, 40);   // 40 맨 끝 삽입
    AL_insertLast(&L, 20);   // 20 맨 끝 삽입
    AL_print(&L);            // 삽입 후 출력

    AL_insertAt(&L, 2, 99);  // 인덱스 2 위치에 99 삽입
    printf("insertAt(2, 99) 후: "); AL_print(&L); // 삽입 후 출력

    AL_deleteAt(&L, 2);      // 인덱스 2 위치 삭제
    printf("deleteAt(2) 후: "); AL_print(&L);     // 삭제 후 출력

    AL_sortAsc(&L);          // 오름차순 정렬
    printf("오름차순 정렬: "); AL_print(&L);      // 정렬 후 출력

    AL_sortDesc(&L);         // 내림차순 정렬
    printf("내림차순 정렬: "); AL_print(&L);      // 정렬 후 출력

    printf("indexOf(30): %d\n", AL_indexOf(&L, 30)); // 30의 위치 탐색
    printf("get(1): %d\n",      (int)AL_get(&L, 1)); // 인덱스 1 값 조회
}

/* ════════════════════════════════════════════════════════════
   demo_single_list : 단순 연결 리스트 데모
   ════════════════════════════════════════════════════════════ */
void demo_single_list(void) {
    printf("\n[=== 단순 연결 리스트 (SingleList) 데모 ===]\n");

    SingleList* L = SL_create();   // 단순 연결 리스트 생성

    printf("insertLast: 1, 2, 3, 4, 5\n"); // 삽입할 값 안내
    SL_insertLast(L, 1);   // 1 맨 끝 삽입
    SL_insertLast(L, 2);   // 2 맨 끝 삽입
    SL_insertLast(L, 3);   // 3 맨 끝 삽입
    SL_insertLast(L, 4);   // 4 맨 끝 삽입
    SL_insertLast(L, 5);   // 5 맨 끝 삽입
    SL_print(L);           // 삽입 후 출력

    SL_insertFront(L, 0);  // 0 앞에 삽입
    printf("insertFront(0) 후: "); SL_print(L); // 삽입 후 출력

    SL_deleteVal(L, 3);    // 값 3 삭제
    printf("deleteVal(3) 후: "); SL_print(L);   // 삭제 후 출력

    SL_reverse(L);         // 리스트 역순
    printf("reverse 후: "); SL_print(L);        // 역순 후 출력

    SL_sortAsc(L);         // 오름차순 정렬
    printf("sortAsc 후: "); SL_print(L);        // 정렬 후 출력

    SL_free(L);            // 전체 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   demo_double_list : 이중 연결 리스트 데모
   ════════════════════════════════════════════════════════════ */
void demo_double_list(void) {
    printf("\n[=== 이중 연결 리스트 (DoubleList) 데모 ===]\n");

    DoubleList* L = DL_create();   // 이중 연결 리스트 생성

    printf("insertLast: 10, 20, 30, 40\n"); // 삽입할 값 안내
    DL_insertLast(L, 10);   // 10 맨 끝 삽입
    DL_insertLast(L, 20);   // 20 맨 끝 삽입
    DL_insertLast(L, 30);   // 30 맨 끝 삽입
    DL_insertLast(L, 40);   // 40 맨 끝 삽입
    DL_printFwd(L);         // 정방향 출력 (head→tail)
    DL_printBwd(L);         // 역방향 출력 (tail→head)

    DL_insertFront(L, 5);   // 5 앞에 삽입
    printf("insertFront(5) 후 정방향: "); DL_printFwd(L); // 정방향 출력

    DL_Node* p = DL_search(L, 20);   // 값 20인 노드 탐색
    if (p) DL_insertAfter(L, p, 25); // 20 뒤에 25 삽입
    printf("insertAfter(20,25) 후: "); DL_printFwd(L); // 삽입 후 출력

    DL_deleteVal(L, 30);   // 값 30 삭제
    printf("deleteVal(30) 후: "); DL_printFwd(L); // 삭제 후 출력

    DL_sortAsc(L);         // 오름차순 정렬
    printf("sortAsc 후: "); DL_printFwd(L); // 정렬 후 출력

    DL_free(L);            // 전체 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   demo_circular_list : 원형 연결 리스트 데모
   ════════════════════════════════════════════════════════════ */
void demo_circular_list(void) {
    printf("\n[=== 원형 연결 리스트 (CircularList) 데모 ===]\n");

    CircularList* CL = CL_create();   // 원형 연결 리스트 생성

    printf("insertLast: 1, 2, 3, 4, 5\n"); // 삽입할 값 안내
    CL_insertLast(CL, 1);   // 1 맨 끝 삽입
    CL_insertLast(CL, 2);   // 2 맨 끝 삽입
    CL_insertLast(CL, 3);   // 3 맨 끝 삽입
    CL_insertLast(CL, 4);   // 4 맨 끝 삽입
    CL_insertLast(CL, 5);   // 5 맨 끝 삽입
    CL_print(CL);           // 원형 리스트 출력

    CL_insertFront(CL, 0);  // 0 앞에 삽입
    printf("insertFront(0) 후: "); CL_print(CL); // 삽입 후 출력

    CL_deleteVal(CL, 3);    // 값 3 삭제
    printf("deleteVal(3) 후: "); CL_print(CL);   // 삭제 후 출력

    CL_sortAsc(CL);         // 오름차순 정렬
    printf("sortAsc 후: "); CL_print(CL);        // 정렬 후 출력

    /* ── 원형 리스트 순환 탐색 예제 (라운드 로빈 느낌) ── */
    printf("\n원형 순환 출력 (3바퀴):\n");   // 순환 출력 안내
    CL_Node* p = CL->head;                   // 시작 노드를 head로 설정
    for (int round = 0; round < 3; round++) {// 3바퀴 순환
        do {                                 // 원형 리스트 한 바퀴
            printf("[%d] ", (int)p->data);   // 노드 데이터 출력
            p = p->link;                     // 다음 노드로 이동
        } while (p != CL->head);             // head로 돌아오면 한 바퀴 완료
        printf("| ");                        // 바퀴 구분자 출력
    }
    printf("\n");

    CL_free(CL);   // 전체 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   main : 모든 모듈 데모 순서대로 실행
   ════════════════════════════════════════════════════════════ */
int main(void) {
    printf("================================================\n");
    printf("  common_modules 범용 자료구조 모듈 데모\n");
    printf("================================================\n");

    demo_fileio();          // 파일 I/O 유틸리티 데모
    demo_array_stack();     // 배열 스택 데모
    demo_linked_stack();    // 연결 스택 데모
    demo_array_queue();     // 배열 원형 큐 데모
    demo_linked_queue();    // 연결 큐 데모
    demo_array_list();      // 배열 순차 리스트 데모
    demo_single_list();     // 단순 연결 리스트 데모
    demo_double_list();     // 이중 연결 리스트 데모
    demo_circular_list();   // 원형 연결 리스트 데모

    printf("\n================================================\n");
    printf("  모든 모듈 데모 완료\n");
    printf("================================================\n");

    getchar();   // Enter 입력 대기
    return 0;    // 정상 종료
}
