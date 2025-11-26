/*
2025-11-26 김현성 notif(알림창) 버그 수정
2025-11-26 김현성 notif(알림창) 작동 확인
2025-11-25 김현성 notif(알림창) 추가
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/notif.h"

extern char g_school_number[50]; // 현재 로그인된 학번

int g_notif_sellected = 1;
char g_notif_lines[MAX_LINES][MAX_CONTENT_LEN]; // 전체 알림 내용 저장
int g_notif_count = 0; // 전체 알림 개수
int g_notif_total_page = 1; // 전체 알림 페이지 수
int g_current_page = 1; // 현재 보고 있는 페이지
int g_from_notif = 0;
char g_id[LINES_PER_PAGE][3][20]; // 각 알림의 id, sender, fileid 저장
char g_title[LINES_PER_PAGE][MAX_TITLE_LEN]; // 각 알림의 제목 저장
char g_notif_content[LINES_PER_PAGE][MAX_LINES]; // 각 알림의 내용 저장
const char g_notif_filename[FILE_NAME_MAX_LEN] = "./data/notif.txt";
bool g_notif_to_DM = false;

// 알림창 화면 그리기
void notif_screen() {
    // 헤더 부분
    draw_rectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    draw_text("Notifications", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);
    draw_rectangle(50, 200, WIDTH / 2 + 570, HEIGHT - 300, TOP_LEFT, BLACK);

    // 버튼 부분
    draw_rectangle(WIDTH - 50, 200, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Clear\n All (C)", WIDTH - 175, 235, 50, MIDDLE_CENTER, BLACK);
    draw_rectangle(WIDTH - 50, 360, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Back (B)", WIDTH - 175, 420, 50, MIDDLE_CENTER, BLACK);

    // 알림 갯수가 0 -> No New Notifications
    if (g_notif_count == 0) {
        draw_text("No New Notifications", WIDTH / 2, HEIGHT / 2, 50, MIDDLE_CENTER, DARKGRAY);
        return;
    }

    // 알림 리스트 부분
    float line_gap = (HEIGHT - 290) / 5;
    for (int i = 0; i < LINES_PER_PAGE; i++) {
        int idx = (g_current_page - 1) * 5 + i;
        if (idx >= g_notif_count) break;

        draw_rectangle(50, 195 + line_gap * i + line_gap / 2, WIDTH / 2 + 570, line_gap - 10, MIDDLE_LEFT, BLACK);
        draw_text(TextFormat("(%d) %s", i + 1, g_id[i][0]), 70, 175 + line_gap * i + line_gap / 2, 40, MIDDLE_LEFT, BLACK);
        draw_text(TextFormat("%s", g_title[i]), 70, 175 + line_gap * i + line_gap / 2 + 40, 40, MIDDLE_LEFT, DARKGRAY);
    }

    // 페이지 표시 부분
    draw_text("<-         ->", WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    draw_text(TextFormat("%d / %d", g_current_page, g_notif_total_page), WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
}

// 알림 파일 읽기
void read_notif_file() {
    // 초기화
    g_notif_count = 0;

    // 파일 열기(없으면 종료->count 0 유지)
    FILE *nf = fopen(g_notif_filename, "r");
    if (nf == NULL) {
        return;
    }

    // 파일에서 한 줄씩 읽기
    while (fgets(g_notif_lines[g_notif_count], sizeof(g_notif_lines[g_notif_count]), nf) != NULL) {
        size_t len = strlen(g_notif_lines[g_notif_count]);
        if (len > 0 && (g_notif_lines[g_notif_count][len - 1] == '\n' || g_notif_lines[g_notif_count][len - 1] == '\r')) {
            g_notif_lines[g_notif_count][len - 1] = '\0';
        }

        // 수신자가 현재 로그인된 사용자일 때 카운트 증가
        sscanf(g_notif_lines[g_notif_count], "%s %s %s", g_id[0][0], g_id[0][1], g_id[0][2]);
        printf("%s %s %d\n", g_school_number, g_id[0][1], strcmp(g_id[0][1], g_school_number));

        if (strcmp(g_id[0][1], g_school_number) == 0)
            g_notif_count++;
    }
    fclose(nf);

    // 각 알림의 제목과 내용 읽어오기
    FILE *df;
    for (int i = 0; i < LINES_PER_PAGE; i++) {
        int idx = (g_current_page - 1) * LINES_PER_PAGE + i;
        if (idx >= g_notif_count) break;

        // notif.txt 데이터 쪼개기(sender, receiver, id)
        sscanf(g_notif_lines[idx], "%s %s %s", g_id[i][0], g_id[i][1], g_id[i][2]);
        printf("id[%d]: %s %s %s\n", i, g_id[i][0], g_id[i][1], g_id[i][2]);

        // 해당 DM 파일 열기
        df = fopen(TextFormat("./data/dm/%s %s %s.txt", g_id[i][0], g_id[i][1], g_id[i][2]), "r");
        fgets(g_title[i], sizeof(g_title[i]), df);
        g_title[i][strcspn(g_title[i], "\r\n")] = '\0';
        fgets(g_notif_content[i], sizeof(g_notif_content[i]), df);
        g_notif_content[i][strcspn(g_notif_content[i], "\r\n")] = '\0';
        
        fclose(df);
        df = NULL;
    }

    // 전체 페이지 수 계산
    if (g_notif_count > 0) {
        g_notif_total_page = (g_notif_count + (LINES_PER_PAGE - 1)) / LINES_PER_PAGE;
    } else {
        g_notif_total_page = 1;
    }
}

// 알림 파일 비우기
void clear_notif() {
    // 기존 파일 열기 (읽기)
    FILE *in = fopen(g_notif_filename, "r");
    if (in == NULL) {
        // 파일이 없으면 변수만 초기화
        g_notif_count = 0;
        g_notif_total_page = 1;
        g_current_page = 1;
        return;
    }

    // 유지할 줄들을 저장하는 배열
    char keep_lines[MAX_LINES][MAX_CONTENT_LEN];
    int keep_count = 0;

    // 파일에서 한 줄씩 읽기
    char line[MAX_CONTENT_LEN];
    char sender[20], receiver[20], msgid[20];

    // 수신자가 내가 아닌 줄만 저장
    while (fgets(line, sizeof(line), in) != NULL) {
        // 수신자가 내가 아니면 유지
        sscanf(line, "%19s %19s %19s", sender, receiver, msgid);
        if (strcmp(receiver, g_school_number) != 0) {
            if (keep_count < MAX_LINES) {
                strncpy(keep_lines[keep_count], line, MAX_CONTENT_LEN - 1);
                keep_lines[keep_count][MAX_CONTENT_LEN - 1] = '\0';
                keep_count++;
            }
        }
    }

    fclose(in);

    // 파일을 다시 열어 덮어쓰기
    FILE *out = fopen(g_notif_filename, "w");
    if (out == NULL) {
        // 파일 열기 실패 시 변수만 초기화
        g_notif_count = 0;
        g_notif_total_page = 1;
        g_current_page = 1;
        return;
    }

    for (int i = 0; i < keep_count; i++) {
        fputs(keep_lines[i], out);
    }
    fclose(out);
    g_notif_count = 0;
    g_notif_total_page = 1;
    g_current_page = 1;
}

// 알림창 초기화
void notif_reset() {
    // 초기 설정
    g_notif_sellected = 1;
    g_notif_count = 0;
    g_notif_total_page = 1;
    g_current_page = 1;

    // 메모리 초기화
    memset(g_notif_lines, 0, sizeof(g_notif_lines));
    memset(g_id, 0, sizeof(g_id));
    memset(g_title, 0, sizeof(g_title));
    memset(g_notif_content, 0, sizeof(g_notif_content));
}
