#include "drone.h"

void run_step_04() {
    FILE* fp4 = fopen("03.txt", "r");
    FILE* fp4w = fopen("04.txt", "a+");
    if (!fp4 || !fp4w) return;

    char p[11], temp[256];
    double dis, k, batteryConsumed, totalBatteryCon = 0;

    fgets(temp, sizeof(temp), fp4);
    fprintf(fp4w, "%10s %10s %10s %11s %11s %10s %10s %11s\n",
        "구간", "거리(m)", " k", "배터리 소모", "누적 소모량", "상태", "이벤트", "안전회귀 경로");
    fprintf(stdout, "%10s %10s %10s %11s %11s %10s %10s %11s\n",
        "구간", "거리(m)", " k", "배터리 소모", "누적 소모량", "상태", "이벤트", "안전회귀 경로");

    for (int i = 0; i < MAX - 1; i++) {
        if (fscanf(fp4, "%10s %10lf %10lf %11lf", p, &dis, &k, &batteryConsumed) != 4) break;

        if (totalBatteryCon + batteryConsumed < MAX_BATTERY / 2.0) {
            if (i == MAX - 2) {
                push(p, dis, k, batteryConsumed);
                totalBatteryCon += batteryConsumed;
                fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, dis, k, batteryConsumed, totalBatteryCon);
                fprintf(fp4w, "%10s %10s %10c\n", "SUCCESS", "폭탄 투하", '-');
                printf("%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, dis, k, batteryConsumed, totalBatteryCon);
                printf("%10s %10s %10c\n", "SUCCESS", "폭탄 투하", '-');
            }
            else {
                push(p, dis, k, batteryConsumed);
                totalBatteryCon += batteryConsumed;
                fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, dis, k, batteryConsumed, totalBatteryCon);
                fprintf(fp4w, "%10s %10c %10c\n", "NORMAL", '-', '-'); fflush(fp4w);
                printf("%10s %10.1lf %10.1lf %10.1lf %10.1lf", p, dis, k, batteryConsumed, totalBatteryCon);
                printf("%10s %10c %10c\n", "NORMAL", '-', '-');
            }
        }
        else {
            totalBatteryCon += batteryConsumed;
            char* na = navigate(NULL);
            fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf %10s %10s %10s\n", p, dis, k, batteryConsumed, totalBatteryCon, "FAILURE", "미션실패", na);
            fprintf(stdout, "%10s %10.1lf %10.1lf %10.1lf %10.1lf %10s %10s %10s\n", p, dis, k, batteryConsumed, totalBatteryCon, "FAILURE", "미션실패", na);
            free(na); break;
        }
    }

    while (!isStackEmpty()) {
        SLL4* node = pop();
        totalBatteryCon += node->batteryCon;
        char rev[3] = { node->point[1], node->point[0], '\0' };
        char path[10]; sprintf(path, "%c→%c", node->point[1], node->point[0]);
        fprintf(fp4w, "%10s %10c %10c %10c %10.1lf %10s %10s %10s\n", rev, '-', '-', '-', totalBatteryCon, "RECOVERY", "역추적", path);
        fprintf(stdout, "%10s %10c %10c %10c %10.1lf %10s %10s %10s\n", rev, '-', '-', '-', totalBatteryCon, "RECOVERY", "역추적", path);
        free(node);
    }

    fclose(fp4); fclose(fp4w);
    printf("===================================================\n");
    printf("\n[안내] 열려있는 메모장(04.txt)을 종료해야 프로그램이 계속됩니다.\n");
    system("notepad.exe 04.txt");
}