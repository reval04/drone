double getDistance(int x1, int y1, int x2, int y2) { // 두 지점 사이 거리 계산
    int dx = x1 - x2;
    int dy = y1 - y2;
    return sqrt((double)(dx * dx + dy * dy));
}
