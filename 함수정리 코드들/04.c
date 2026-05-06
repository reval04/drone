#include "drone.h"

int main() {
    // 1~3단계 실행
    makeWayPoint();
    arrangeWayPoint();
    makeDistancePerBatteryTable();

    FILE* fp4 = fopen("03.txt", "r");
    FILE* fp4w = fopen("04.txt", "w");
    if (!fp4 || !fp4w) return -1;

    char p[11], temp[256];
    double dis, k, bc, totalBC = 0;

    fgets(temp, sizeof(temp), fp4); // 헤더 스킵

    printIndex(fp42w);

    for (int i = 0; i < MAX - 1; i++) {
        if (fscanf(fp4, "%10s %10lf %10lf %11lf", p, &dis, &k, &bc) != 4) break;

        if (totalBC + bc < MAX_B / 2.0) {
            push(p, dis, k, bc);
            totalBC += bc;
            fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf %10s %10c %10c\n", p, dis, k, bc, totalBC, "NORMAL", '-', '-');
            fprintf(stdout, "%10s %10.1lf %10.1lf %10.1lf %10.1lf %10s %10c %10c\n", p, dis, k, bc, totalBC, "NORMAL", '-', '-');
        }
        else {
            totalBC += bc;
            char* na = navigate(NULL);
            fprintf(fp4w, "%10s %10.1lf %10.1lf %10.1lf %10.1lf %10s %10s %10s\n", p, dis, k, bc, totalBC, "FAILURE", "미션실패", na);
            fprintf(stdout, "%10s %10.1lf %10.1lf %10.1lf %10.1lf %10s %10s %10s\n", p, dis, k, bc, totalBC, "FAILURE", "미션실패", na);
            free(na); break;
        }
    }

    while (!isStackEmpty()) {
        SLL4* node = pop();
        totalBC += node->batteryCon;
        char rev[3] = { node->point[1], node->point[0], '\0' };
        char path[10]; sprintf(path, "%c→%c", node->point[1], node->point[0]);
        fprintf(fp4w, "%10s %10c %10c %10c %10.1lf %10s %10s %10s\n", rev, '-', '-', '-', totalBC, "RECOVERY", "역추적", path);
        fprintf(stdout, "%10s %10c %10c %10c %10.1lf %10s %10s %10s\n", rev, '-', '-', '-', totalBC, "RECOVERY", "역추적", path);
        free(node);
    }

    fclose(fp4); fclose(fp4w);
    system("notepad.exe 04.txt");
    return 0;
}
