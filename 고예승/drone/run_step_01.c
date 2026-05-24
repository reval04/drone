#include "drone.h"

void run_step_01() {
    FILE* fp1 = fopen("01.txt", "a+");
    if (fp1 == NULL) return;

    printf("\n웨이포인트 생성 결과:\n");

    // 파일과 화면 상단에 컬럼 구분 헤더 추가
    fprintf(fp1, "%8s %8s\n", "X", "Y");
    printf("%8s %8s\n", "X", "Y");

    fprintf(fp1, "%8d %8d\n", 0, 0);
    printf("%8d %8d\n", 0, 0);

    int i, x, y;
    for (i = 0; i < MAX - 1; i++) {
        x = rand() % MAX_RAND;
        y = rand() % MAX_RAND;

        fprintf(fp1, "%8d %8d\n", x, y);
        printf("%8d %8d\n", x, y);
    }

    fclose(fp1);
    printf("===================================================\n");
    printf("\n[안내] 열려있는 메모장(01.txt)을 종료해야 다음 절차로 진행됩니다.\n");
    system("notepad.exe 01.txt");
}