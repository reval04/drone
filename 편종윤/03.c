#define _CRT_SECURE_NO_WARNINGS
#include "drone.h"

extern int current_drone_id;

void run_step_03() {
    FILE* fp = fopen("02.txt", "r");
    if (fp == NULL) return;

    char ids[6];
    int pts[6][2];
    // Z를 포함하여 총 6개(Z, A, B, C, D, E)의 노드를 읽음
    for (int i = 0; i < 6; i++) {
        if (fscanf(fp, "%c: %d %d\n", &ids[i], &pts[i][0], &pts[i][1]) != 3) {}
    }
    fclose(fp);

    FILE* out = fopen("03.txt", "a");
    if (out == NULL) return;

    fprintf(out, "droneId | 구간 | 거리(m) | K | 배터리 소모\n");
    printf("\n배터리 소모 테이블 예측 결과:\n");
    printf("droneId | 구간 | 거리(m) | K | 배터리 소모\n");

    double total_dist = 0.0;
    double total_consumed = 0.0;

    for (int i = 0; i < 5; i++) {
        double dist = sqrt(pow((double)pts[i + 1][0] - (double)pts[i][0], 2.0) +
            pow((double)pts[i + 1][1] - (double)pts[i][1], 2.0));

        double k = (dist >= 200.0) ? 1.7 : 1.5;
        double consumed = dist * k;

        total_dist += dist;
        total_consumed += consumed;

        fprintf(out, "%04d | %c%c | %.1f | %.1f | %.1f\n", current_drone_id, ids[i], ids[i + 1], dist, k, consumed);
        printf("%04d | %c%c | %.1f | %.1f | %.1f\n", current_drone_id, ids[i], ids[i + 1], dist, k, consumed);
    }

    fprintf(out, "%04d | TOTAL | %.1f | - | %.1f\n", current_drone_id, total_dist, total_consumed);
    printf("%04d | TOTAL | %.1f | - | %.1f\n", current_drone_id, total_dist, total_consumed);

    fprintf(out, "--------------------------------------------------\n");
    fclose(out);

    printf("\n[안내] 열려있는 메모장(03.txt)을 종료하면 작전이 시작됩니다.\n");
    system("notepad.exe 03.txt");
}