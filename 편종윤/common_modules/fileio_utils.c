#define _CRT_SECURE_NO_WARNINGS   // MSVC 보안 경고 억제
#include <stdio.h>                // FILE, printf, fopen, fgets 등
#include <stdlib.h>               // malloc, free
#include <string.h>               // strlen, strcpy
#include <stdarg.h>               // va_list, va_start, va_end (가변 인자)
#include "fileio_utils.h"         // 파일 입출력 유틸리티 헤더 포함

/* ════════════════════════════════════════════════════════════
   fileExists : 파일 존재 여부 확인
   fopen으로 열어보고 성공 여부로 판단
   ════════════════════════════════════════════════════════════ */
int fileExists(const char* filename) {
    FILE* fp = fopen(filename, "r");   // 읽기 모드로 열기 시도
    if (fp) {                          // 열기 성공 = 파일 존재
        fclose(fp);                    // 파일 닫기
        return 1;                      // 존재: 1 반환
    }
    return 0;                          // 없음: 0 반환
}

/* ════════════════════════════════════════════════════════════
   fileSize : 파일 크기(바이트) 반환
   fseek으로 끝으로 이동 후 ftell로 위치(=크기) 확인
   ════════════════════════════════════════════════════════════ */
long fileSize(const char* filename) {
    FILE* fp = fopen(filename, "rb"); // 이진 읽기 모드로 열기
    if (!fp) return -1L;              // 열기 실패 시 -1 반환
    fseek(fp, 0, SEEK_END);          // 파일 끝으로 이동
    long size = ftell(fp);           // 현재 위치 = 파일 크기(바이트)
    fclose(fp);                      // 파일 닫기
    return size;                     // 크기 반환
}

/* ════════════════════════════════════════════════════════════
   countLines : 파일의 줄(행) 수 반환
   '\n' 문자 개수를 세어 줄 수 결정
   ════════════════════════════════════════════════════════════ */
int countLines(const char* filename) {
    FILE* fp = fopen(filename, "r");   // 읽기 모드로 열기
    if (!fp) return 0;                  // 열기 실패 시 0 반환
    int lines = 0;                      // 줄 수 카운터
    int ch;                             // 읽은 문자
    while ((ch = fgetc(fp)) != EOF) {  // 파일 끝까지 한 문자씩 읽기
        if (ch == '\n') lines++;        // 개행 문자 발견 시 줄 수 증가
    }
    fclose(fp);                         // 파일 닫기
    return lines;                       // 줄 수 반환
}

/* ════════════════════════════════════════════════════════════
   readIntArray : 파일에서 정수 배열 읽기
   첫 줄: 개수 N, 이후 N개의 정수를 arr에 저장
   반환값: 실제로 읽은 정수 개수
   ════════════════════════════════════════════════════════════ */
int readIntArray(const char* filename, int* arr, int maxCount) {
    FILE* fp = fopen(filename, "r");   // 읽기 모드로 열기
    if (!fp) {                          // 열기 실패 시
        printf("[오류] 파일 열기 실패: %s\n", filename); // 오류 출력
        return 0;                        // 0 반환
    }
    int count = 0;                       // 읽을 개수
    fscanf(fp, "%d", &count);           // 첫 줄에서 개수 읽기
    if (count > maxCount) count = maxCount; // 최대치 초과 방지
    for (int i = 0; i < count; i++)    // count 개 만큼 반복
        fscanf(fp, "%d", &arr[i]);      // 정수 하나씩 읽어 배열에 저장
    fclose(fp);                          // 파일 닫기
    return count;                        // 읽은 개수 반환
}

/* ════════════════════════════════════════════════════════════
   writeIntArray : 파일에 정수 배열 쓰기
   첫 줄에 개수 N, 이후 N개의 정수를 한 줄씩 기록
   ════════════════════════════════════════════════════════════ */
void writeIntArray(const char* filename, const int* arr, int count) {
    FILE* fp = fopen(filename, "w");   // 쓰기 모드로 열기
    if (!fp) {                          // 열기 실패 시
        printf("[오류] 파일 열기 실패: %s\n", filename); // 오류 출력
        return;                          // 함수 종료
    }
    fprintf(fp, "%d\n", count);        // 첫 줄에 개수 기록
    for (int i = 0; i < count; i++)    // count 개 만큼 반복
        fprintf(fp, "%d\n", arr[i]);   // 정수 하나씩 기록
    fclose(fp);                         // 파일 닫기
}

/* ════════════════════════════════════════════════════════════
   readLines : 파일에서 문자열 배열 읽기
   각 줄을 lines[i]에 저장, 반환값: 읽은 줄 수
   ════════════════════════════════════════════════════════════ */
int readLines(const char* filename, char lines[][128], int maxLines) {
    FILE* fp = fopen(filename, "r");    // 읽기 모드로 열기
    if (!fp) {                           // 열기 실패 시
        printf("[오류] 파일 열기 실패: %s\n", filename); // 오류 출력
        return 0;                         // 0 반환
    }
    int count = 0;                        // 읽은 줄 수
    while (count < maxLines &&            // 최대 줄 수 초과 전까지
           fgets(lines[count], 128, fp))  // 한 줄씩 읽기
    {
        int len = (int)strlen(lines[count]); // 읽은 줄 길이
        if (len > 0 && lines[count][len-1] == '\n') // 끝에 개행 있으면
            lines[count][len-1] = '\0';   // 개행 문자 제거
        count++;                           // 줄 수 증가
    }
    fclose(fp);                            // 파일 닫기
    return count;                          // 읽은 줄 수 반환
}

/* ════════════════════════════════════════════════════════════
   appendLine : 파일 끝에 한 줄 추가 (append 모드)
   ════════════════════════════════════════════════════════════ */
void appendLine(const char* filename, const char* line) {
    FILE* fp = fopen(filename, "a");   // 추가(append) 모드로 열기
    if (!fp) {                          // 열기 실패 시
        printf("[오류] 파일 열기 실패: %s\n", filename); // 오류 출력
        return;                          // 함수 종료
    }
    fprintf(fp, "%s\n", line);         // 내용 + 개행 추가
    fclose(fp);                         // 파일 닫기
}

/* ════════════════════════════════════════════════════════════
   printHeader : 보고서 헤더 출력 (=== 제목 ===)
   fp가 NULL이면 stdout(화면)에만 출력
   ════════════════════════════════════════════════════════════ */
void printHeader(FILE* fp, const char* title) {
    char buf[128];                              // 출력 버퍼
    int len = (int)strlen(title) + 8;          // 구분선 길이 계산
    /* ── 구분선 생성 ── */
    for (int i = 0; i < len; i++) buf[i] = '=';// '=' 문자로 채우기
    buf[len] = '\0';                            // 문자열 종료
    printf("%s\n  %s\n%s\n", buf, title, buf); // 화면 출력
    if (fp) fprintf(fp, "%s\n  %s\n%s\n",      // 파일 출력 (fp 있을 때만)
                    buf, title, buf);
}

/* ════════════════════════════════════════════════════════════
   printDivider : 구분선 출력 (width 개의 '-')
   ════════════════════════════════════════════════════════════ */
void printDivider(FILE* fp, int width) {
    for (int i = 0; i < width; i++) {   // width 만큼 반복
        printf("-");                     // 화면에 '-' 출력
        if (fp) fprintf(fp, "-");        // 파일에 '-' 출력
    }
    printf("\n");                        // 화면 개행
    if (fp) fprintf(fp, "\n");          // 파일 개행
}

/* ════════════════════════════════════════════════════════════
   printFooter : 보고서 하단 출력
   ════════════════════════════════════════════════════════════ */
void printFooter(FILE* fp) {
    printf("\n[ 보고서 끝 ]\n");           // 화면 출력
    if (fp) fprintf(fp, "\n[ 보고서 끝 ]\n"); // 파일 출력
}

/* ════════════════════════════════════════════════════════════
   fprintBoth : printf + fprintf 동시 수행
   화면과 파일에 동일한 내용을 한 번에 출력
   사용법: fprintBoth(fp, "값: %d\n", val);
   ════════════════════════════════════════════════════════════ */
void fprintBoth(FILE* fp, const char* fmt, ...) {
    va_list args;                    // 가변 인자 리스트
    va_start(args, fmt);             // 가변 인자 시작 (fmt 이후부터)
    vprintf(fmt, args);              // 화면(stdout)에 가변 인자 출력
    va_end(args);                    // 가변 인자 종료

    if (fp) {                        // 파일 포인터가 있으면
        va_start(args, fmt);         // 가변 인자 재시작
        vfprintf(fp, fmt, args);     // 파일에 가변 인자 출력
        va_end(args);                // 가변 인자 종료
    }
}
