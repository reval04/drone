#include "drone.h"

/* ============================================================
 *  05.c  —  Step 0 ~ Step 3
 *  Step 0 : 큐잉이론 (작전 지속 가능 여부 판단)
 *  Step 1 : 충전 대기열 (원형 큐, FIFO)
 *  Step 2 : 위험 구간 분석 정렬 (BST 중위 순회)
 *  Step 3 : 관제망 통신 보고 (이진 트리 전위/후위 순회)
 * ============================================================ */


/* ============================================================
 *  Step 0 — 큐잉이론 (M/M/1 모델)
 * ============================================================ */
void step0_queueing(double lambda, double mu)
{
    printf("\n[Step 0] 큐잉이론 — 작전 지속 가능 여부 판단\n");
    printf("  도착률 lambda = %.2f, 처리율 mu = %.2f\n", lambda, mu);

    double rho = lambda / mu;
    printf("  부하율 rho = %.4f\n", rho);

    if (rho >= 1.0)
    {
        printf("  >> 베이스캠프 과부하 경고: 추가 출격 대기\n");
        return;
    }

    double Lq = (rho * rho) / (1.0 - rho);
    double W  = 1.0 / (mu - lambda);
    printf("  평균 대기 드론 수 Lq = %.2f\n", Lq);
    printf("  평균 체류 시간   W  = %.2f\n", W);

    if (Lq > QUEUE_CAP)
    {
        printf("  >> 베이스캠프 과부하 경고: 추가 출격 대기\n");
        printf("     (예측 대기열 %.2f > 큐 용량 %d)\n", Lq, QUEUE_CAP);
    }
    else
    {
        printf("  >> 작전 지속 가능: 정상 운용 범위\n");
    }
}


/* ============================================================
 *  Step 1 — 충전 대기열 (원형 큐)
 * ============================================================ */
void queueInit(ChargeQueue* q)
{
    q->front = 0;
    q->rear  = 0;
    q->size  = 0;
}

static int queueFull(ChargeQueue* q)
{
    if (q->size == QUEUE_CAP) return 1;
    return 0;
}

static int queueEmpty(ChargeQueue* q)
{
    if (q->size == 0) return 1;
    return 0;
}

int enqueue(ChargeQueue* q, char* name, double used)
{
    if (queueFull(q)) return 0;

    strcpy(q->droneName[q->rear], name);
    q->batteryUsed[q->rear] = used;
    q->rear = (q->rear + 1) % QUEUE_CAP;
    (q->size)++;
    return 1;
}

int dequeue(ChargeQueue* q, char* outName, double* outUsed)
{
    if (queueEmpty(q)) return 0;

    strcpy(outName, q->droneName[q->front]);   /* 배열 복사 */
    *outUsed = q->batteryUsed[q->front];
    q->front = (q->front + 1) % QUEUE_CAP;
    (q->size)--;                               /* 꺼냈으니 감소 */
    return 1;
}


/* ============================================================
 *  Step 2 — 위험 구간 분석 BST
 * ============================================================ */
static BSTNode* bstNewNode(char* seg, double con)
{
    BSTNode* n = (BSTNode*)malloc(sizeof(BSTNode));
    strcpy(n->segment, seg);
    n->consumption = con;
    n->left = n->right = NULL;
    return n;
}

BSTNode* bstInsert(BSTNode* root, char* seg, double con)
{
    if (root == NULL)
    {
        root = bstNewNode(seg, con);
        return root;
    }

    if (con < root->consumption)
        root->left  = bstInsert(root->left,  seg, con);
    else
        root->right = bstInsert(root->right, seg, con);

    return root;
}

void bstInorder(BSTNode* root)
{
    if (root == NULL) return;
    bstInorder(root->left);
    printf("  구간 %-4s  소모량 %.1f\n", root->segment, root->consumption);
    bstInorder(root->right);
}

void bstFree(BSTNode* root)
{
    if (root == NULL) return;
    bstFree(root->left);
    bstFree(root->right);
    free(root);
}


/* ============================================================
 *  Step 3 — 웨이포인트 이진 트리
 *           A
 *          / \
 *         B   C
 *        / \
 *       D   E
 * ============================================================ */
TreeNode* treeNew(char name)
{
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    n->name = name;
    n->left = n->right = NULL;        /* 오타 수정: left/right */
    return n;
}

void preorder(TreeNode* node)
{
    if (node == NULL) return;
    printf("  %c 지점 통신 연결 확인\n", node->name);
    preorder(node->left);
    preorder(node->right);
}

void postorder(TreeNode* node)
{
    if (node == NULL) return;
    postorder(node->left);
    postorder(node->right);
    printf("  %c 지점 이상무 -> 상위 보고\n", node->name);
}

void treeFree(TreeNode* node)
{
    if (node == NULL) return;
    treeFree(node->left);
    treeFree(node->right);
    free(node);
}


/* ============================================================
 *  통합 실행 함수 — 04.c 의 main 끝에서 호출
 * ============================================================ */
void runStep0123()
{
    printf("\n===================================================\n");
    printf("            Step 0 ~ 3 추가 작전 시작\n");
    printf("===================================================\n");

    /* ---------- Step 0 ---------- */
    step0_queueing(LAMBDA, MU);

    /* ---------- 04.txt 읽기 ---------- */
    FILE* fp = fopen("04.txt", "r");
    if (fp == NULL) {
        printf("04.txt 파일을 열 수 없습니다.\n");
        return;
    }

    char header[256];
    fgets(header, sizeof(header), fp);   /* 헤더 한 줄 건너뛰기 */

    /* Step 1 큐 / Step 2 BST 준비 */
    ChargeQueue q;
    queueInit(&q);
    BSTNode* bstRoot = NULL;

    char   line[256];
    char   seg[12], state[20], event[20], path[20];
    double dis, k, batteryCon, totalCon;
    int    count = 0;

    /* ---------- 한 줄씩 읽으면서 Step 1, 2 처리 ----------
       04.txt 는 줄마다 '-' 가 섞여 있어 fscanf 형식이 깨진다.
       그래서 fgets 로 한 줄 통째로 읽고, 맨 앞 토큰 2개
       (구간명, 배터리소모)만 sscanf 로 뽑는다.
       NORMAL/SUCCESS 줄은 4번째 필드가 숫자,
       RECOVERY 줄은 '-' 라서 숫자 변환이 실패 -> 건너뜀.
    */
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        /* 구간명 + (거리 k 배터리소모) 순으로 시도 */
        if (sscanf(line, "%11s %lf %lf %lf",
                   seg, &dis, &k, &batteryCon) == 4)
        {
            /* Step 1: 충전 대기열 등록 */
            enqueue(&q, seg, batteryCon);
            /* Step 2: BST 삽입 */
            bstRoot = bstInsert(bstRoot, seg, batteryCon);
            count++;
        }
        /* 숫자 변환 실패(RECOVERY 등)면 그냥 무시하고 다음 줄 */
    }
    fclose(fp);

    if (count == 0)
        printf("\n(읽어들인 비행 구간이 없습니다)\n");

    /* ---------- Step 1 결과 출력 ---------- */
    printf("\n[Step 1] 충전 대기열 처리 (FIFO)\n");
    {
        char   outName[12];
        double outUsed;
        int    no = 1;
        while (dequeue(&q, outName, &outUsed))
        {
            printf("  #%d 드론(%s) 충전 완료 — 소모 %.1f, 배터리 리셋\n",
                   no, outName, outUsed);
            no++;
        }
    }

    /* ---------- Step 2 결과 출력 ---------- */
    printf("\n[Step 2] 위험 구간 분석 (안전 -> 위험 오름차순)\n");
    bstInorder(bstRoot);
    bstFree(bstRoot);

    /* ---------- Step 3 ---------- */
    printf("\n[Step 3] 관제망 통신 및 보고\n");
    {
        TreeNode* root    = treeNew('A');
        root->left        = treeNew('B');
        root->right       = treeNew('C');
        root->left->left  = treeNew('D');
        root->left->right = treeNew('E');

        printf("  -- 미션 시작 전 (전위 순회) --\n");
        preorder(root);
        printf("  -- 미션 종료 후 (후위 순회) --\n");
        postorder(root);

        treeFree(root);
    }

    printf("\n===================================================\n");
    printf("            Step 0 ~ 3 작전 종료\n");
    printf("===================================================\n");
}
