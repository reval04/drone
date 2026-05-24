#include "drone.h"

int current_drone_id = 1000;

int main() {
    srand((unsigned int)time(NULL));
    int choice;

    while (1) {
        printf("\n=== 군사용 전술 드론 통제 시스템 ===\n");
        printf("1. 드론 폭파 미션 생성\n");
        printf("0. Systems 종료\n");
        printf("선택: ");

        if (scanf("%d", &choice) != 1) break;

        if (choice == 0) {
            char confirm;
            printf("정말 종료하시겠습니까? (Y/N): ");
            scanf(" %c", &confirm);
            if (confirm == 'Y' || confirm == 'y') {
                printf("통제 시스템을 종료합니다.\n");
                break;
            }
            else {
                continue;
            }
        }

        if (choice == 1) {
            current_drone_id = rand() % 9000 + 1000;

            run_step_01();
            run_step_02();

            while (1) {
                printf("\n1. 작전 웨이포인트 재생성\n");
                printf("2. 작전 진행\n");
                printf("0. 시스템 종료\n");
                printf("선택: ");

                int sub_choice;
                if (scanf("%d", &sub_choice) != 1) break;

                if (sub_choice == 0) {
                    char confirm;
                    printf("정말 종료하시겠습니까? (Y/N): ");
                    scanf(" %c", &confirm);
                    if (confirm == 'Y' || confirm == 'y') {
                        printf("통제 시스템을 종료합니다.\n");
                        return 0;
                    }
                    else {
                        continue;
                    }
                }

                if (sub_choice == 1) {
                    current_drone_id = rand() % 9000 + 1000;
                    run_step_01();
                    run_step_02();
                    continue;
                }

                if (sub_choice == 2) {
                    run_step_03();
                    run_step_04();

                    printf("\n작전이 종료되었습니다. 초기 화면으로 돌아갑니다.\n");
                    break;
                }
            }
        }
    }
    return 0;
}

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