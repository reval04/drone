#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // printf, FILE, fopen 등
#include <stdlib.h>               // rand(), srand()
#include <time.h>                 // time() 난수 시드용
#include "droneStack.h"           // 드론 스택 헤더 포함

#define MAX_STEPS       15   // 최대 비행 스텝 수 (전진 한계)
#define BATTERY_START  100   // 초기 배터리 잔량 (100%)
#define BATTERY_LOW     20   // 이 값 이하가 되면 귀환 시작 (20%)
#define MOVE_COST_MIN    3   // 스텝당 최소 배터리 소모량
#define MOVE_COST_MAX   12   // 스텝당 최대 배터리 소모량
#define COORD_STEP      10   // 한 스텝 이동 거리 단위 (m)

/* ════════════════════════════════════════════════════════════
   stage1_generate
   목적: 난수로 비행 경로 데이터 생성 → flight_data.txt 저장
   형식: step dx dy battery_cost (한 줄씩)
   ════════════════════════════════════════════════════════════ */
void stage1_generate(const char* filename) {
    FILE* fp = fopen(filename, "w");       // 쓰기 모드로 파일 열기
    if (!fp) {                             // 파일 열기 실패 확인
        printf("파일 열기 실패: %s\n", filename); // 오류 메시지
        return;                            // 함수 종료
    }
    fprintf(fp, "%d\n", MAX_STEPS);        // 첫 줄에 총 스텝 수 기록

    for (int i = 1; i <= MAX_STEPS; i++) { // 각 스텝마다 데이터 생성
        int dx   = (rand() % 3 - 1) * COORD_STEP;  // X축 이동: -10, 0, +10 중 하나
        int dy   = (rand() % 3 - 1) * COORD_STEP;  // Y축 이동: -10, 0, +10 중 하나
        int cost = MOVE_COST_MIN + rand() % (MOVE_COST_MAX - MOVE_COST_MIN + 1); // 소모량 난수
        fprintf(fp, "%d %d %d %d\n", i, dx, dy, cost); // step, dx, dy, cost 기록
    }
    fclose(fp);                            // 파일 닫기
    printf("[Stage 1] 비행 데이터 %d 스텝 생성 → %s\n", MAX_STEPS, filename); // 완료 안내
}

/* ════════════════════════════════════════════════════════════
   stage2_simulate
   목적: flight_data.txt 읽기 → 전진 시 push / 배터리 부족 시 pop 귀환
         → return_log.txt 에 시뮬레이션 로그 저장
   ════════════════════════════════════════════════════════════ */
void stage2_simulate(const char* input, const char* output) {
    FILE* fp = fopen(input, "r");      // 비행 데이터 파일 읽기 모드로 열기
    if (!fp) {                         // 열기 실패 확인
        printf("파일 열기 실패: %s\n", input); return; // 오류 후 종료
    }
    FILE* log = fopen(output, "w");    // 로그 파일 쓰기 모드로 열기
    if (!log) {                        // 열기 실패 확인
        printf("파일 열기 실패: %s\n", output); fclose(fp); return; // 오류 후 종료
    }

    Stack* path = createStack();       // 비행 경로를 저장할 스택 생성
    int total_steps;                   // 파일에 기록된 총 스텝 수
    fscanf(fp, "%d", &total_steps);    // 첫 줄에서 총 스텝 수 읽기

    int battery       = BATTERY_START; // 현재 배터리를 100%로 초기화
    int cx = 0, cy = 0;               // 현재 위치 초기화 (베이스 = 원점)
    int forward_steps = 0;            // 전진한 스텝 수 카운터
    int return_steps  = 0;            // 귀환한 스텝 수 카운터

    /* ── 로그 파일 헤더 작성 ── */
    fprintf(log, "=== 비행 시뮬레이션 로그 ===\n\n");           // 제목
    fprintf(log, "[전진 단계]\n");                               // 섹션 구분
    fprintf(log, "%-6s %-8s %-8s %-10s %-10s\n",               // 열 헤더 (형식 맞춤)
            "Step", "X", "Y", "배터리(%)", "동작");
    fprintf(log, "----------------------------------------------\n"); // 구분선

    /* ── 화면 헤더 출력 ── */
    printf("\n[Stage 2] 비행 시뮬레이션 시작 (초기 배터리: %d%%)\n\n", battery);
    printf("%-6s %-8s %-8s %-10s %-10s\n", "Step", "X", "Y", "배터리(%)", "동작");
    printf("----------------------------------------------\n");

    /* ── 전진 루프: 파일에서 스텝 데이터 읽으며 스택에 push ── */
    int step, dx, dy, cost;                                   // 한 스텝의 이동 데이터
    while (fscanf(fp, "%d %d %d %d", &step, &dx, &dy, &cost) == 4) { // 4개 읽기 성공 시
        battery -= cost;                                      // 배터리 소모 차감
        if (battery < 0) battery = 0;                        // 음수 방지 (최소 0)
        cx += dx;                                            // X 좌표 이동
        cy += dy;                                            // Y 좌표 이동

        Position pos = {step, cx, cy, battery};              // 현재 위치 구조체 생성
        push(path, pos);                                     // 현재 위치를 스택에 push
        forward_steps++;                                     // 전진 스텝 수 증가

        /* ── 화면 및 로그 파일에 한 줄 기록 ── */
        printf("%-6d %-8d %-8d %-10d %-10s\n", step, cx, cy, battery, "전진");
        fprintf(log, "%-6d %-8d %-8d %-10d %-10s\n", step, cx, cy, battery, "전진");

        if (battery <= BATTERY_LOW) {                        // 배터리가 임계값 이하이면
            printf("  *** 배터리 부족! (%d%%) 귀환 시작 ***\n", battery); // 경고 출력
            fprintf(log, "\n  *** 배터리 부족! (%d%%) 귀환 시작 ***\n\n", battery);
            break;                                           // 전진 루프 탈출
        }
    }
    fclose(fp);  // 비행 데이터 파일 닫기

    /* ── 귀환 로그 헤더 작성 ── */
    fprintf(log, "\n[귀환 단계]\n");
    fprintf(log, "%-6s %-8s %-8s %-10s %-10s\n", "Step", "X", "Y", "배터리(%)", "동작");
    fprintf(log, "----------------------------------------------\n");
    printf("\n[귀환 단계]\n");
    printf("%-6s %-8s %-8s %-10s %-10s\n", "Step", "X", "Y", "배터리(%)", "동작");
    printf("----------------------------------------------\n");

    int avg_cost   = (MOVE_COST_MIN + MOVE_COST_MAX) / 2 / 2; // 귀환 시 소모량 (전진 평균의 절반)
    int ret_step   = forward_steps + 1;                        // 귀환 스텝 번호 시작값

    pop(path);  // 현재 위치(top)를 제거하고 이전 위치부터 역추적 시작

    /* ── 귀환 루프: 스택 pop하며 역순 경로로 귀환 ── */
    while (!isStackEmpty(path)) {               // 스택에 노드가 있는 동안
        Position prev = pop(path);             // 이전 위치를 스택에서 꺼내기
        battery -= avg_cost;                   // 귀환 중 배터리 소모
        if (battery < 0) battery = 0;         // 음수 방지

        printf("%-6d %-8d %-8d %-10d %-10s\n",       // 귀환 스텝 화면 출력
               ret_step, prev.x, prev.y, battery, "귀환");
        fprintf(log, "%-6d %-8d %-8d %-10d %-10s\n", // 귀환 스텝 로그 기록
                ret_step, prev.x, prev.y, battery, "귀환");
        ret_step++;        // 스텝 번호 증가
        return_steps++;    // 귀환 스텝 수 증가

        if (battery == 0) break;  // 배터리 완전 방전이면 강제 중단
    }

    /* ── 귀환 성공 여부 판정 ── */
    int returned = (isStackEmpty(path) && battery > 0) ? 1 : 0; // 스택 소진+배터리 남으면 성공

    /* ── 최종 결과 로그 기록 ── */
    fprintf(log, "\n[최종 결과]\n");
    fprintf(log, "전진 스텝 수  : %d\n", forward_steps);   // 전진 스텝 수 기록
    fprintf(log, "귀환 스텝 수  : %d\n", return_steps);    // 귀환 스텝 수 기록
    fprintf(log, "잔여 배터리   : %d%%\n", battery);       // 남은 배터리 기록
    fprintf(log, "귀환 성공 여부: %s\n",                   // 성공/실패 기록
            returned ? "성공" : "배터리 방전 - 실패");
    fclose(log);  // 로그 파일 닫기

    printf("\n귀환 %s | 잔여 배터리: %d%%\n",
           returned ? "성공" : "실패(방전)", battery);     // 화면 결과 출력
    printf("[Stage 2] 로그 저장 → %s\n", output);         // 저장 파일명 안내

    freeStack(path);  // 스택 메모리 해제
}

/* ════════════════════════════════════════════════════════════
   stage3_report
   목적: return_log.txt 분석 → battery_report.txt 보고서 생성
   ════════════════════════════════════════════════════════════ */
void stage3_report(const char* input, const char* output) {
    FILE* fp = fopen(input, "r");   // 로그 파일 읽기 모드로 열기
    if (!fp) { printf("파일 열기 실패: %s\n", input); return; } // 실패 시 종료

    FILE* rp = fopen(output, "w");  // 보고서 파일 쓰기 모드로 열기
    if (!rp) { printf("파일 열기 실패: %s\n", output); fclose(fp); return; } // 실패 시 종료

    char line[128];                   // 한 줄 읽기용 버퍼
    int  forward_steps = 0;           // 전진 스텝 수 저장 변수
    int  return_steps  = 0;           // 귀환 스텝 수 저장 변수
    int  remaining     = 0;           // 잔여 배터리 저장 변수

    /* ── 로그 파일에서 요약 수치 파싱 ── */
    while (fgets(line, sizeof(line), fp)) {                       // 한 줄씩 읽기
        sscanf(line, "전진 스텝 수  : %d", &forward_steps);      // 전진 스텝 수 파싱
        sscanf(line, "귀환 스텝 수  : %d", &return_steps);       // 귀환 스텝 수 파싱
        sscanf(line, "잔여 배터리   : %d", &remaining);          // 잔여 배터리 파싱
    }
    fclose(fp);  // 로그 파일 닫기

    int total_steps  = forward_steps + return_steps;              // 전체 스텝 수 계산
    int used_battery = BATTERY_START - remaining;                 // 소모한 배터리 계산

    /* ── 보고서 내용 작성 ── */
    fprintf(rp, "=== 드론 배터리 귀환 보고서 ===\n\n");          // 제목
    fprintf(rp, "초기 배터리     : %d%%\n",   BATTERY_START);    // 초기값 기록
    fprintf(rp, "배터리 임계값   : %d%%\n",   BATTERY_LOW);      // 귀환 기준 기록
    fprintf(rp, "전진 스텝 수    : %d\n",     forward_steps);    // 전진 스텝 기록
    fprintf(rp, "귀환 스텝 수    : %d\n",     return_steps);     // 귀환 스텝 기록
    fprintf(rp, "총 스텝 수      : %d\n",     total_steps);      // 총 스텝 기록
    fprintf(rp, "소모 배터리     : %d%%\n",   used_battery);     // 소모량 기록
    fprintf(rp, "잔여 배터리     : %d%%\n",   remaining);        // 잔량 기록
    if (total_steps > 0)                                          // 나눗셈 전 0 체크
        fprintf(rp, "스텝당 평균 소모: %.1f%%\n",
                (double)used_battery / total_steps);              // 평균 소모량 기록
    fprintf(rp, "귀환 성공 여부  : %s\n",
            (remaining > 0) ? "성공" : "배터리 방전 - 실패");    // 최종 판정 기록
    fclose(rp);  // 보고서 파일 닫기

    printf("\n[Stage 3] 보고서 저장 → %s\n", output);  // 저장 완료 안내
}

/* ════════════════════════════════════════════════════════════
   main : 3단계 파이프라인 순서대로 실행
   ════════════════════════════════════════════════════════════ */
int main(void) {
    srand((unsigned int)time(NULL));  // 현재 시각으로 난수 시드 초기화

    printf("================================================\n");
    printf("  드론 배터리 귀환 시스템 (연결 리스트 스택)\n");
    printf("================================================\n\n");

    stage1_generate("flight_data.txt");                     // Stage 1: 비행 데이터 생성
    stage2_simulate("flight_data.txt", "return_log.txt");   // Stage 2: 비행 시뮬레이션
    stage3_report  ("return_log.txt",  "battery_report.txt");// Stage 3: 보고서 생성

    printf("\n================================================\n");
    printf("  파이프라인 완료\n");
    printf("  flight_data.txt → return_log.txt → battery_report.txt\n");
    printf("================================================\n");

    getchar();   // Enter 입력 대기 (화면 유지)
    return 0;    // 정상 종료
}
