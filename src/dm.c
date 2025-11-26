#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/dm.h"


int g_dm_selected = -1;
int g_DMs = 0;
int g_myDMs = 0;
int g_DM_page = 1;
char g_myDM_list[128][256];
int *g_dm_search_result;
int g_dm_search_result_length = 0;
int g_DM_types = 1;
bool g_DM_sync = false;

char g_DM_school_number[50];

extern DMList *g_dm_messages;

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

void DM_reset() {
    g_dm_selected = 0;
    g_DMs = 0;
    g_myDMs = 0;
    g_DM_page = 1;
    g_DM_sync = false;
    g_DM_types = 1;
    memset(g_myDM_list, 0, sizeof(g_myDM_list));
    memset(g_DM_school_number, 0, sizeof(g_DM_school_number));
}

void DMListSync() {
    g_DM_page = 1;
    g_dm_search_result = g_dm_messages->search_message(g_dm_messages, g_DM_school_number, g_DM_types);
    g_dm_search_result_length = dm_list_length(g_dm_search_result);
}