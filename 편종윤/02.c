#define _CRT_SECURE_NO_WARNINGS
#include "drone.h"

void run_step_02() {
    FILE* fp = fopen("01.txt", "r");
    if (fp == NULL) return;

    int pts[5][2];
    for (int i = 0; i < 5; i++) {
        if (fscanf(fp, "%d %d", &pts[i][0], &pts[i][1]) != 2) {}
    }
    fclose(fp);

    int visited[5] = { 0 };
    int curr_x = 0, curr_y = 0;
    char names[] = { 'A', 'B', 'C', 'D', 'E' };

    LinkedList* L = createList();
    FILE* out = fopen("02.txt", "w");
    if (out == NULL) {
        freeList(L);
        return;
    }

    printf("\n최단 거리 기반 작전 경로 구성 결과:\n");

    // 통제실 Z는 원점
    insertLast(L, 'Z', 0, 0);
    fprintf(out, "Z: 0 0\n");
    printf("Z: 0 0\n");

    for (int step = 0; step < 5; step++) {
        int best_idx = -1;
        double min_dist = 1e9;

        for (int i = 0; i < 5; i++) {
            if (!visited[i]) {
                double dist = sqrt(pow((double)pts[i][0] - (double)curr_x, 2.0) +
                    pow((double)pts[i][1] - (double)curr_y, 2.0));
                if (dist < min_dist) {
                    min_dist = dist;
                    best_idx = i;
                }
            }
        }

        if (best_idx != -1) {
            visited[best_idx] = 1;
            curr_x = pts[best_idx][0];
            curr_y = pts[best_idx][1];

            insertLast(L, names[step], curr_x, curr_y);
            fprintf(out, "%c: %d %d\n", names[step], curr_x, curr_y);
            printf("%c: %d %d\n", names[step], curr_x, curr_y);
        }
    }

    fclose(out);
    freeList(L);

    printf("\n[안내] 열려있는 메모장(02.txt)을 종료해야 다음 절차로 진행됩니다.\n");
    system("notepad.exe 02.txt");
}