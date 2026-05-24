#include "drone.h"

void run_step_03() {
    double dis[MAX - 1];
    FILE* fp3r = fopen("02.txt", "r");
    if (!fp3r) return;

    // [중요] 02.txt 파일 맨 위의 헤더 문자열("이름 X Y") 한 줄 읽어서 건너뛰기
    char header_buffer[256];
    fgets(header_buffer, sizeof(header_buffer), fp3r);

    SLLH* nodeHead = createSingleLinkedList();

    int i;
    int x, y;
    char name;
    for (i = 0; i < MAX; i++) {
        if (fscanf(fp3r, " %c %d %d", &name, &x, &y) != 3) break;
        insertLastNode(nodeHead, x, y, name);
    }
    fclose(fp3r);

    SLL* temp = nodeHead->head;
    for (i = 0; i < MAX - 1; i++) {
        dis[i] = distance(temp);
        temp = temp->next;
    }

    temp = nodeHead->head;
    printDistance(dis, temp);
    freeLinkedList(nodeHead);
    printf("===================================================\n");
    printf("\n[안내] 열려있는 메모장(03.txt)을 종료해야 다음 절차로 진행됩니다.\n");
    system("notepad.exe 03.txt");
}