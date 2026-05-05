#define _CRT_SECURE_NO_WARNINGS
#include "drone.h"

extern int current_drone_id;

void get_return_path(Stack* s, char* buffer) {
    buffer[0] = '\0';
    if (s == NULL) return;

    StackNode* curr = s->top;
    while (curr) {
        char temp[10];
        sprintf(temp, "%c", curr->data.id);
        strcat(buffer, temp);
        curr = curr->link;
        if (curr) strcat(buffer, "->");
    }
}

void run_step_04() {
    FILE* fp = fopen("02.txt", "r");
    if (fp == NULL) return;

    char ids[6];
    int pts[6][2];

    for (int i = 0; i < 6; i++) {
        if (fscanf(fp, "%c: %d %d\n", &ids[i], &pts[i][0], &pts[i][1]) != 3) {}
    }
    fclose(fp);

    double total_needed = 0;
    // 5구간 소모량 도출
    for (int i = 0; i < 5; i++) {
        double d = sqrt(pow((double)pts[i + 1][0] - (double)pts[i][0], 2.0) +
            pow((double)pts[i + 1][1] - (double)pts[i][1], 2.0));
        total_needed += d * ((d >= 200.0) ? 1.7 : 1.5);
    }

    double MAX_BATTERY = total_needed * 1.3;
    double threshold = MAX_BATTERY * 0.5;
    double current_battery = MAX_BATTERY;
    double cum_consumed = 0.0;
    int failed = 0;

    Stack* s = createStack();
    if (s == NULL) return;

    FILE* out = fopen("04.txt", "a");
    if (out != NULL) {
        fprintf(out, "droneId | 구간 | 거리(m) | K | 배터리 소모량 | 누적 소모량 | 상태 | 이벤트 | 안전회귀 경로\n");
        fclose(out);
    }

    printf("droneId | 구간 | 거리(m) | K | 배터리 소모량 | 누적 소모량 | 상태 | 이벤트 | 안전회귀 경로\n");

    // 원점 삽입
    Position start = { ids[0], pts[0][0], pts[0][1], cum_consumed, NORMAL };
    push(s, start);

    for (int i = 0; i < 5; i++) {
        double dist = sqrt(pow((double)pts[i + 1][0] - (double)pts[i][0], 2.0) +
            pow((double)pts[i + 1][1] - (double)pts[i][1], 2.0));
        double k = (dist >= 200.0) ? 1.7 : 1.5;
        double consumed = dist * k;

        if (current_battery - consumed < threshold) {
            char path_buf[100];
            get_return_path(s, path_buf);

            out = fopen("04.txt", "a");
            if (out != NULL) {
                fprintf(out, "%04d | %c%c | %.1f | %.1f | %.1f | %.1f | FAILURE | 미션실패 | %c->%s\n",
                    current_drone_id, ids[i], ids[i + 1], dist, k, consumed, cum_consumed, ids[i + 1], path_buf);
                fclose(out);
            }

            printf("%04d | %c%c | %.1f | %.1f | %.1f | %.1f | FAILURE | 미션실패 | %c->%s\n",
                current_drone_id, ids[i], ids[i + 1], dist, k, consumed, cum_consumed, ids[i + 1], path_buf);

            printf("\n[안내] 열려있는 메모장(04.txt)을 종료하면 통제실로 복귀합니다.\n");
            system("notepad.exe 04.txt");

            failed = 1;
            break;

        }
        else {
            cum_consumed += consumed;
            current_battery -= consumed;

            Position p = { ids[i + 1], pts[i + 1][0], pts[i + 1][1], cum_consumed, NORMAL };
            push(s, p);

            out = fopen("04.txt", "a");
            if (out != NULL) {
                fprintf(out, "%04d | %c%c | %.1f | %.1f | %.1f | %.1f | NORMAL | - | -\n",
                    current_drone_id, ids[i], ids[i + 1], dist, k, consumed, cum_consumed);
                fclose(out);
            }

            printf("%04d | %c%c | %.1f | %.1f | %.1f | %.1f | NORMAL | - | -\n",
                current_drone_id, ids[i], ids[i + 1], dist, k, consumed, cum_consumed);

            printf("\n[안내] 열려있는 메모장(04.txt)을 종료해야 다음 경로로 기동합니다.\n");
            system("notepad.exe 04.txt");
        }
    }

    if (failed) {
        Position curr_pos = pop(s);

        while (!isStackEmpty(s)) {
            Position next_pos = pop(s);

            double dist = sqrt(pow((double)curr_pos.x - (double)next_pos.x, 2.0) +
                pow((double)curr_pos.y - (double)next_pos.y, 2.0));
            double k = (dist >= 200.0) ? 1.7 : 1.5;
            double consumed = dist * k;

            cum_consumed += consumed;

            char path_buf[50];
            sprintf(path_buf, "%c->%c", curr_pos.id, next_pos.id);

            out = fopen("04.txt", "a");
            if (out != NULL) {
                fprintf(out, "%04d | %c%c(회귀) | %.1f | %.1f | %.1f | %.1f | RECOVERY | 역추적 | %s\n",
                    current_drone_id, curr_pos.id, next_pos.id, dist, k, consumed, cum_consumed, path_buf);
                fclose(out);
            }

            printf("%04d | %c%c(회귀) | %.1f | %.1f | %.1f | %.1f | RECOVERY | 역추적 | %s\n",
                current_drone_id, curr_pos.id, next_pos.id, dist, k, consumed, cum_consumed, path_buf);

            printf("\n[안내] 열려있는 메모장(04.txt)을 종료해야 다음 회귀 경로로 기동합니다.\n");
            system("notepad.exe 04.txt");

            curr_pos = next_pos;
        }

        out = fopen("04.txt", "a");
        if (out != NULL) {
            // 복귀 도착지를 통제실 Z로 명시
            fprintf(out, ">>> [통제실] 드론(ID:%04d)이 임무를 중단하고 통제실로 안전하게 복귀를 완수했습니다. <<<\n", current_drone_id);
            fprintf(out, "---------------------------------------------------------------------------------\n");
            fclose(out);
        }
        printf("\n>>> [통제실] 드론(ID:%04d)이 임무를 중단하고 통제실로 안전하게 복귀를 완수했습니다. <<<\n", current_drone_id);

    }
    else {
        out = fopen("04.txt", "a");
        if (out != NULL) {
            fprintf(out, ">>> [통제실] 드론(ID:%04d)이 최종 목적지에 도달하여 폭파 임무를 완수했습니다! <<<\n", current_drone_id);
            fprintf(out, "---------------------------------------------------------------------------------\n");
            fclose(out);
        }
        printf("\n>>> [통제실] 드론(ID:%04d)이 최종 목적지에 도달하여 폭파 임무를 완수했습니다! <<<\n", current_drone_id);
    }

    printf("\n[안내] 메모장을 닫으면 초기 화면으로 돌아갑니다.\n");
    system("notepad.exe 04.txt");

    freeStack(s);
}