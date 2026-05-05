#define _CRT_SECURE_NO_WARNINGS
#include "drone.h"
#include <time.h>

int current_drone_id = 1000; // 드론 식별용 변수

int main() {
    srand((unsigned)time(NULL));
    int choice;

    while (1) {
        printf("\n=== 군사용 전술 드론 통제 시스템 ===\n");
        printf("1. 드론 폭파 미션 생성\n");
        printf("0. 시스템 종료\n");
        printf("선택: ");

        if (scanf("%d", &choice) != 1) break;

        // 종료 확인
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
            // 새로 시작할 때마다 드론 ID 발급 (범위: 1000 ~ 9999)
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

                // 종료 확인
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
    FILE* fp = fopen("01.txt", "w");
    if (fp == NULL) return;

    printf("\n웨이포인트 생성 결과:\n");
    for (int i = 0; i < 5; i++) {
        int x = rand() % 501;
        int y = rand() % 501;
        fprintf(fp, "%d %d\n", x, y);

        // CMD 동시 출력
        printf("%d %d\n", x, y);
    }
    fclose(fp);

    // 메모장 대기 안내
    printf("\n[안내] 열려있는 메모장(01.txt)을 종료해야 다음 절차로 진행됩니다.\n");
    system("notepad.exe 01.txt");
}