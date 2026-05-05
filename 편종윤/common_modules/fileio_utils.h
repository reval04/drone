/* ════════════════════════════════════════════════════════════
   fileio_utils.h  —  파일 입출력 유틸리티 모듈 헤더
   ─────────────────────────────────────────────────────────
   어느 프로젝트에서나 재사용 가능한 파일 I/O 보조 함수 모음.
   정수 배열 읽기/쓰기, 줄 수 세기, 파일 존재 확인,
   보고서 헤더/구분선 출력 등을 제공한다.
   ════════════════════════════════════════════════════════════ */

#pragma once          // 헤더 중복 포함 방지
#include <stdio.h>    // FILE, printf 등
#include <stdlib.h>   // malloc, free

/* ── 파일 존재/크기 확인 ── */
int   fileExists    (const char* filename);                // 파일이 존재하면 1, 없으면 0
long  fileSize      (const char* filename);                // 파일 크기(바이트) 반환
int   countLines    (const char* filename);                // 파일의 줄(행) 수 반환

/* ── 정수 배열 읽기/쓰기 ── */
int   readIntArray  (const char* filename,                 // 파일에서 정수 배열 읽기
                     int* arr, int maxCount);              //   arr: 저장 배열, 반환값: 읽은 개수
void  writeIntArray (const char* filename,                 // 파일에 정수 배열 쓰기
                     const int* arr, int count);           //   count: 저장할 개수

/* ── 문자열(라인) 읽기/쓰기 ── */
int   readLines     (const char* filename,                 // 파일에서 문자열 배열 읽기
                     char lines[][128], int maxLines);     //   반환값: 읽은 줄 수
void  appendLine    (const char* filename,                 // 파일 끝에 한 줄 추가 (append)
                     const char* line);

/* ── 보고서 출력 헬퍼 ── */
void  printHeader   (FILE* fp, const char* title);         // 보고서 헤더 출력 (=== 제목 ===)
void  printDivider  (FILE* fp, int width);                 // 구분선 출력 (--- 길이만큼 ---)
void  printFooter   (FILE* fp);                            // 보고서 하단 출력

/* ── 화면+파일 동시 출력 ── */
void  fprintBoth    (FILE* fp, const char* fmt, ...);      // printf + fprintf 동시 수행
