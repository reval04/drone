typedef struct Node {
    char name;
    int x,y;
    struct Node* link;
} Node;

typedef enum { NORMAL, FAILURE, RECOVERY } situation;
typedef struct Drone {
    int id;
    int currentBattety;     // 현재 배터리
    Node* head;
    situation state;
    struct HealthNode* next; //
} Drone;


double getDistance(int x1, int y1, int x2, int y2);// 두 지점 사이 거리 계산
void addCoordinate(const char* filename); // 좌표 추가