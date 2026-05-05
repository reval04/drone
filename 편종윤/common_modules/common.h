/* ════════════════════════════════════════════════════════════
   common.h  —  범용 자료구조 모듈 마스터 헤더
   ─────────────────────────────────────────────────────────
   이 헤더 하나만 include 하면 아래 모든 모듈을 사용할 수 있다.
   각 모듈을 개별적으로 사용하고 싶으면 해당 헤더만 포함해도 된다.

   [element 타입 변경 방법]
     이 파일 상단의 typedef 한 줄만 수정하면 모든 모듈에 반영됨.
     예)  typedef double element;   → 실수형
          typedef char   element;   → 문자형
          단, 구조체 타입은 각 모듈 헤더에서 직접 수정 권장.
   ════════════════════════════════════════════════════════════ */

#pragma once   // 헤더 중복 포함 방지

/* ── 공통 element 타입 정의 (프로젝트에 맞게 수정) ── */
#ifndef ELEMENT_TYPE_DEFINED     // element 타입이 아직 정의되지 않은 경우
#define ELEMENT_TYPE_DEFINED     // 정의 플래그 설정
typedef int element;             // 기본 element 타입: int (필요 시 변경)
#endif

/* ── 표준 라이브러리 공통 포함 ── */
#include <stdio.h>    // printf, FILE, fopen 등 입출력
#include <stdlib.h>   // malloc, free, rand, srand 등
#include <string.h>   // strcpy, strcmp, memset 등
#include <time.h>     // time() 난수 시드용

/* ── 각 모듈 헤더 포함 ── */
#include "fileio_utils.h"   // 파일 입출력 유틸리티
#include "array_stack.h"    // 배열 기반 스택
#include "linked_stack.h"   // 연결 리스트 기반 스택
#include "array_queue.h"    // 배열 기반 원형 큐
#include "linked_queue.h"   // 연결 리스트 기반 큐
#include "array_list.h"     // 배열 기반 순차 리스트
#include "single_list.h"    // 단순 연결 리스트
#include "double_list.h"    // 이중 연결 리스트
#include "circular_list.h"  // 원형 연결 리스트
