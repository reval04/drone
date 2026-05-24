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

