#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/dm.h"


int g_DM_page = 1; // DM 페이지
int *g_dm_search_result; // DM 이름으로 찾기
int g_dm_search_result_length = 0; // 조건에 맞는 DM 개수
int g_DM_types = 1; // 1: Send, 2: Inbox
bool g_DM_sync = false; // 처음 DM 불러오기

char g_DM_school_number[50];

extern DMList *g_dm_messages;

//DM 화면 띄우기
void DMscreen() {
    draw_rectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    draw_text("DM", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);

    draw_rectangle(50, 200, WIDTH / 2 + 570, HEIGHT - 300, TOP_LEFT, BLACK); // 큰상자
    draw_text("<-         ->", WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    int denominator = g_dm_search_result_length / 7 + (g_dm_search_result_length % 7 ? 1 : 0);
    draw_text(TextFormat("%d / %d", g_DM_page, (denominator ? denominator : 1)), WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);

    int result_height = (HEIGHT - 290) / 7;
    for (int i = 0; i < 7; i++) {
        int ind = (g_DM_page - 1) * 7 + i;
        if (ind < g_dm_search_result_length) {
            draw_rectangle(50, 195 + result_height * i + result_height / 2, WIDTH / 2 + 570, result_height - 10, MIDDLE_LEFT, BLACK);
            draw_text(TextFormat("(%d) %s", i + 1, g_dm_messages->list[g_dm_search_result[ind]]->title), 70, 195 + result_height * i + result_height / 2, 40, MIDDLE_LEFT, BLACK);
        }
    }
    
        
    // 보낸 편지함

    draw_rectangle(WIDTH - 50, 200, 250, 120, TOP_RIGHT, (g_DM_types == 1 ? RED : BLACK));
    draw_text("Sent (S)", WIDTH - 175, 260, 50, MIDDLE_CENTER, BLACK);

    // 받은 편지함
    draw_rectangle(WIDTH - 50, 360, 250, 120, TOP_RIGHT, (g_DM_types == 2 ? RED : BLACK));
    draw_text("Inbox (I)", WIDTH - 175, 420, 50, MIDDLE_CENTER, BLACK);

    // 뒤로가기
    draw_rectangle(WIDTH - 50, 520, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Back (B)", WIDTH - 175, 580, 50, MIDDLE_CENTER, BLACK);
}

// DM 창 끄기
void DM_reset() {
    g_DM_page = 1;
    g_DM_sync = false;
    g_DM_types = 1;
    memset(g_DM_school_number, 0, sizeof(g_DM_school_number));
}

// DM 불러오기(한 번만 실행)
void DMListSync() {
    g_DM_page = 1;
    g_dm_search_result = g_dm_messages->search_message(g_dm_messages, g_DM_school_number, g_DM_types);
    g_dm_search_result_length = dm_list_length(g_dm_search_result);
}