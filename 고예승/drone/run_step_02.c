#include "drone.h"

void run_step_02() {
    FILE* fp2 = fopen("01.txt", "r");
    if (!fp2) return;

    // [중요] 01.txt 파일 맨 위의 헤더 문자열("X Y") 한 줄 읽어서 건너뛰기
    char header_buffer[256];
    fgets(header_buffer, sizeof(header_buffer), fp2);

    SLLH* head = createSingleLinkedList();
    SLL* temp;

    int i, x, y;
    for (i = 0; i < MAX; i++) {
        if (fscanf(fp2, "%d %d", &x, &y) != 2) break;
        insertLastNode(head, x, y, NULL);
    }
    fclose(fp2);

    bubbleSort(head);
    pinPointing(head);

    FILE* fp2w = fopen("02.txt", "a+");
    if (!fp2w) {
        freeLinkedList(head);
        return;
    }

    // 02.txt 파일과 화면 상단에 명확한 구조 구분을 위한 헤더 추가
    fprintf(fp2w, "%8s %8s %8s\n", "이름", "X", "Y");
    printf("%8s %8s %8s\n", "이름", "X", "Y");

    temp = head->head;
    while (temp != NULL) {
        fprintf(fp2w, "%8c %8d %8d\n", temp->name, temp->x, temp->y);
        printf("%8c %8d %8d\n", temp->name, temp->x, temp->y);
        temp = temp->next;
    }
    fclose(fp2w);
    freeLinkedList(head);
    printf("===================================================\n");
    printf("\n[안내] 열려있는 메모장(02.txt)을 종료해야 다음 절차로 진행됩니다.\n");
    system("notepad.exe 02.txt");
}