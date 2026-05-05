double getDistance(int x1, int y1, int x2, int y2) { // 두 지점 사이 거리 계산
    int dx = x1 - x2;
    int dy = y1 - y2;
    return sqrt((double)(dx * dx + dy * dy));
}

void addCoordnate(const char* filename) { 
    FILE* f = fopen(filename, "a+");
    if (f == NULL) return;
    for (int i = 0; i < MAX; i++) {
        int x = rand() % 500;
        int y = rand() % 500;
        fprintf(stdout, "(%d,%d)    ", x, y);
        fprintf(f, "(%d,%d)    ", x, y);
    }
    fclose(f);
}
