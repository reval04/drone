#include "drone.h"

extern int current_drone_id;
extern double force_weight;

void run_step_05() {
    printf("\n===================================================\n");
    printf(" [Step 5] 드론 복귀 후 사후 분석 및 정비 시스템\n");
    printf("===================================================\n");


    // [Step 5-1] 관제망 통신 점검 (이진트리 전위 순회)
    printf("\n=== [통신망 점검] 관제탑 -> 각 지점 (하향식 확인) ===\n");
    TreeNode* rootNode = createTreeNode('A');
    rootNode->left = createTreeNode('B');
    rootNode->right = createTreeNode('C');
    rootNode->left->left = createTreeNode('D');
    rootNode->left->right = createTreeNode('E');

    preOrder(rootNode);
    printf("통신 연결 양호!\n");


    // [Step 5-2] 04.txt 파일 데이터 위험도 분석 (BST 중위 순회)
    printf("\n=== [위험 구간 분석] 배터리 소모량 기준 오름차순 정렬 ===\n");
    FILE* fp = fopen("04.txt", "r");
    BSTNode* bstRoot = NULL;

    if (fp != NULL) {
        char line[256];
        // 첫 번째 줄(헤더) 스킵
        fgets(line, sizeof(line), fp);

        // 04.txt 배터리 소모량이 있는 구간만 추출
        while (fgets(line, sizeof(line), fp) != NULL) {
            char section[20];
            double dist, k, bat;

            // sscanf가 정상적으로 4개의 값(구간, 거리, k, 소모량)을 읽어내면 BST에 삽입
            // (RECOVERY 등 배터리 소모량이 '-' 인 줄은 자동으로 걸러짐)
            if (sscanf(line, "%s %lf %lf %lf", section, &dist, &k, &bat) == 4) {
                bstRoot = insertBST(bstRoot, section, bat);
            }
        }
        fclose(fp);

        printf("가장 안전한 구간부터 위험한 구간 순으로 정렬되었습니다.\n");
        inOrderBST(bstRoot);
    }
    else {
        printf("[오류] 04.txt 파일을 찾을 수 없습니다.\n");
    }


    // [Step 5-3] 관제망 최종 보고 (이진트리 후위 순회)
    printf("\n=== [복귀 완료] 각 지점 -> 관제탑 (상향식 상태 보고) ===\n");
    postOrder(rootNode);
    printf("최종 보고 완료!\n");


    // [Step 5-4] 정비소 대기열 시스템 (큐잉 이론 & 원형 큐)
    printf("\n=== [정비소 대기열 시스템 가동] ===\n");

    // 기본 도착률과 처리율 랜덤 설정
    double lambda_base = (rand() % 5) + 3.0;
    double mu = (rand() % 5) + 4.0;

    // [핵심] 사용자의 강행 가중치를 현재 도착률에 합산!
    double lambda = lambda_base + force_weight;

    if (force_weight > 0) {
        printf("[시스템 알림] 이전 작전 '강행'으로 인해 복귀 드론 도착률에 가중치(+%.1lf)가 적용되었습니다!\n", force_weight);
    }

    // 반환된 부하율(rho)을 받음
    double rho = checkQueueingTheory(lambda, mu);

    Queue baseCampQueue;
    initQueue(&baseCampQueue);

    Drone myDrone = { current_drone_id, 0.0 };
    enqueue(&baseCampQueue, myDrone);

    // 부하율(rho)에 따른 분기 처리
    if (rho < 1.0) {
        // 1. 시스템 안정: 바로 충전 완료 처리
        dequeue(&baseCampQueue);

        if (force_weight > 0) {
            printf("기지의 정비 능력이 여유로워 밀린 대기열이 모두 해소되었습니다.");
            printf("가중치가 0으로 초기화됩니다.");
        }
    }
    else {
        // 2. 과부하 상태
        if (rho >= 1.8) {
            // 부하율이 너무 높으면 충전 불가, 큐에서 나가지 못함(Dequeue 안 함)
            printf(">> [상태] 드론 %d호기: 정비 공간 포화 상태! 충전 '대기중'...\n", myDrone.droneID);
        }
        else {
            // 그 외 과부하는 일단 충전 처리
            dequeue(&baseCampQueue);
        }

        // 과부하 시 지휘관 결단 메뉴 팝업
        int q_choice;
        printf("\n[지휘관 결단] 기지 시스템 과부하 상태입니다. 다음 작전 방침을 선택하세요.\n");
        printf("1. 강행 (과부하 악화 위험)\n");
        printf("2. 잠깐 대기 (시스템 안정화 및 대기열 즉시 해소)\n");
        printf("선택: ");

        if (scanf("%d", &q_choice) == 1) {
            if (q_choice == 1) {
                printf("\n휴식 없이 작전을 강행합니다!\n");
                force_weight += 2.0; // 가중치 2.0 누적
            }
            else {
                printf("\n기지 정비 시간을 확보합니다.\n");
                printf("[시스템 알림] 밀린 대기열이 모두 해소되었습니다.\n");
                force_weight = 0.0; // 가중치 리셋

                // '대기중'이던 드론이 있었다면, 충전
                if (rho >= 1.8) {
                    printf("\n[시스템 알림] 정비 공간이 확보되어 대기 중이던 드론의 충전을 재개합니다.\n");
                    dequeue(&baseCampQueue);
                }
            }
        }
    }

    printf("===================================================\n");
}