#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 함수 경고 억제
#include <stdio.h>                // printf, FILE, fopen, fprintf 등
#include <stdlib.h>               // rand(), srand(), exit()
#include <time.h>                 // time() - 난수 시드용
#include <math.h>                 // sqrt() - 거리 계산용
#include "droneList.h"            // 드론 경유지 연결 리스트 헤더

#define NUM_WAYPOINTS  10    // 생성할 총 경유지 개수
#define DELETE_COUNT    3    // Stage 2에서 삭제할 경유지 개수
#define COORD_RANGE   100    // 좌표 범위 (−100 ~ +100 m)
#define ALT_MAX       200    // 고도 최대값 (0 ~ 200 m)

/* ════════════════════════════════════════════════════════════
   stage1_generate
   목적: 난수로 경유지 데이터를 생성하여 waypoints.txt에 저장
   파이프라인: [난수 생성] → waypoints.txt
   ════════════════════════════════════════════════════════════ */
void stage1_generate(const char* filename) {
    FILE* fp = fopen(filename, "w");    // 쓰기 모드로 출력 파일 열기
    if (!fp) {                          // 파일 열기 실패 확인
        printf("파일 열기 실패: %s\n", filename); // 오류 메시지 출력
        return;                         // 함수 종료
    }
    fprintf(fp, "%d\n", NUM_WAYPOINTS); // 첫 줄에 총 경유지 수 기록

    for (int i = 1; i <= NUM_WAYPOINTS; i++) {   // 경유지 수만큼 반복
        int x   = (rand() % (COORD_RANGE * 2 + 1)) - COORD_RANGE; // −100~100 난수 좌표 X
        int y   = (rand() % (COORD_RANGE * 2 + 1)) - COORD_RANGE; // −100~100 난수 좌표 Y
        int alt = rand() % (ALT_MAX + 1);         // 0~200 난수 고도
        fprintf(fp, "%d %d %d %d\n", i, x, y, alt); // id, x, y, 고도를 파일에 기록
    }
    fclose(fp);                                   // 파일 닫기
    printf("[Stage 1] 경유지 %d개 생성 → %s\n", NUM_WAYPOINTS, filename); // 완료 안내
}

/* ════════════════════════════════════════════════════════════
   stage2_process
   목적: waypoints.txt를 읽어 연결 리스트 구성 → 일부 삭제 → path_result.txt 저장
   파이프라인: waypoints.txt → [연결 리스트 + 삭제] → path_result.txt
   ════════════════════════════════════════════════════════════ */
void stage2_process(const char* input, const char* output) {
    linkedList_h* L = createLinkedList();  // 빈 연결 리스트 생성

    printf("\n[Stage 2] 입력 파일 로드: %s\n", input); // 로드 시작 안내
    loadFromFile(L, input);                // 파일에서 경유지 데이터를 리스트에 적재

    printf("\n초기 경로:\n");              // 초기 상태 출력 안내
    printList(L);                         // 현재 리스트 내용 화면 출력

    printf("\n[Stage 2] 경유지 %d개 삭제 중...\n", DELETE_COUNT); // 삭제 시작 안내

    for (int i = 0; i < DELETE_COUNT; i++) {             // 삭제 횟수만큼 반복
        int target = (rand() % NUM_WAYPOINTS) + 1;       // 1~NUM_WAYPOINTS 중 무작위 ID 선택
        if (deleteNodeById(L, target))                   // 해당 ID 노드 삭제 시도
            printf("  삭제 성공: ID %d\n", target);      // 삭제 성공 메시지
        else                                             // 이미 삭제된 경우
            printf("  삭제 실패 (없음): ID %d\n", target); // 실패 메시지
    }

    printf("\n최종 경로:\n");             // 처리 후 상태 출력 안내
    printList(L);                        // 삭제 후 리스트 출력
    saveToFile(L, output);               // 최종 리스트를 파일에 저장
    freeLinkedList(L);                   // 리스트 메모리 전체 해제
}

/* ════════════════════════════════════════════════════════════
   stage3_summary
   목적: path_result.txt를 읽어 총 이동 거리 계산 → path_summary.txt 보고서 생성
   파이프라인: path_result.txt → [거리 계산] → path_summary.txt
   ════════════════════════════════════════════════════════════ */
void stage3_summary(const char* input, const char* output) {
    linkedList_h* L = createLinkedList();  // 빈 연결 리스트 생성

    printf("\n[Stage 3] 입력 파일 로드: %s\n", input); // 로드 시작 안내
    loadFromFile(L, input);                // 파일에서 최종 경로 데이터 로드

    double dist = calcTotalDistance(L);    // 모든 경유지 간 3D 거리 합산

    FILE* fp = fopen(output, "w");         // 보고서 파일 쓰기 모드로 열기
    if (!fp) {                             // 파일 열기 실패 처리
        printf("파일 열기 실패: %s\n", output); // 오류 메시지
        freeLinkedList(L);                 // 메모리 해제 후
        return;                            // 함수 종료
    }

    fprintf(fp, "=== 드론 경로 요약 보고서 ===\n\n");      // 보고서 제목 기록
    fprintf(fp, "총 경유지 수  : %d개\n", L->size);        // 경유지 수 기록
    fprintf(fp, "총 이동 거리  : %.2f m\n\n", dist);       // 합산 거리 기록
    fprintf(fp, "%-6s %-6s %-8s %-8s %-8s\n",             // 열 헤더 출력 (형식 맞춤)
            "순서", "ID", "X좌표", "Y좌표", "고도(m)");
    fprintf(fp, "--------------------------------------\n"); // 구분선 기록

    listNode* p = L->head;   // 순회 포인터를 첫 노드로 설정
    int seq = 1;             // 출력 순서 번호 초기화
    while (p != NULL) {      // 모든 노드 순회
        fprintf(fp, "%-6d %-6d %-8d %-8d %-8d\n",         // 순서, ID, 좌표, 고도 기록
                seq++, p->id, p->x, p->y, p->altitude);
        p = p->link;         // 다음 노드로 이동
    }
    fclose(fp);              // 보고서 파일 닫기

    printf("\n[Stage 3] 결과 요약:\n");                // 화면 요약 출력
    printf("  총 경유지: %d개\n", L->size);           // 경유지 수 출력
    printf("  총 거리 : %.2f m\n", dist);             // 총 거리 출력
    printf("  → 저장: %s\n", output);                 // 파일명 안내

    freeLinkedList(L);       // 사용 완료된 리스트 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   main : 3단계 파이프라인을 순서대로 실행
   ════════════════════════════════════════════════════════════ */
int main(void) {
    srand((unsigned int)time(NULL));  // 현재 시각으로 난수 시드 설정 (매번 다른 결과)

    printf("========================================\n");
    printf("  드론 경로 관리 시스템 (단순 연결 리스트)\n");
    printf("========================================\n\n");

    stage1_generate("waypoints.txt");                     // Stage 1: 난수 경유지 생성
    stage2_process("waypoints.txt", "path_result.txt");   // Stage 2: 리스트 구성 및 삭제
    stage3_summary("path_result.txt", "path_summary.txt");// Stage 3: 거리 계산 및 보고서

    printf("\n========================================\n");
    printf("  파이프라인 완료\n");
    printf("  waypoints.txt → path_result.txt → path_summary.txt\n");
    printf("========================================\n");

    getchar();   // 프로그램 종료 전 화면 유지 (Enter 입력 대기)
    return 0;    // 정상 종료
}
