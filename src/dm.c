#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/dm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int dm_selected = -1;
int DMs = 0;
int myDMs = 0;
int DM_page = 1;
char myDM_list[128][256];
int *dm_search_result;
int dm_search_result_length = 0;
int DM_types = 1;
bool DM_sync = false;

char DM_school_number[50];

extern DMList *dm_messages;

void DMscreen() {
    draw_rectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    draw_text("DM", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);

    draw_rectangle(50, 200, WIDTH / 2 + 570, HEIGHT - 300, TOP_LEFT, BLACK); // 큰상자
    draw_text("<-         ->", WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    int denominator = dm_search_result_length / 7 + (dm_search_result_length % 7 ? 1 : 0);
    draw_text(TextFormat("%d / %d", DM_page, (denominator ? denominator : 1)), WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);

    int result_height = (HEIGHT - 290) / 7;
    for (int i = 0; i < 7; i++) {
        int ind = (DM_page - 1) * 7 + i;
        if (ind < dm_search_result_length) {
            draw_rectangle(50, 195 + result_height * i + result_height / 2, WIDTH / 2 + 570, result_height - 10, MIDDLE_LEFT, BLACK);
            draw_text(TextFormat("(%d) %s", i + 1, dm_messages->list[dm_search_result[ind]]->title), 70, 195 + result_height * i + result_height / 2, 40, MIDDLE_LEFT, BLACK);
        }
    }
    
        
    // 보낸 편지함

    draw_rectangle(WIDTH - 50, 200, 250, 120, TOP_RIGHT, (DM_types == 1 ? RED : BLACK));
    draw_text("Sent (S)", WIDTH - 175, 260, 50, MIDDLE_CENTER, BLACK);

    // 받은 편지함
    draw_rectangle(WIDTH - 50, 360, 250, 120, TOP_RIGHT, (DM_types == 2 ? RED : BLACK));
    draw_text("Inbox (I)", WIDTH - 175, 420, 50, MIDDLE_CENTER, BLACK);

    // 뒤로가기
    draw_rectangle(WIDTH - 50, 520, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Back (B)", WIDTH - 175, 580, 50, MIDDLE_CENTER, BLACK);
}

void DM_reset() {
    dm_selected = 0;
    DMs = 0;
    myDMs = 0;
    DM_page = 1;
    DM_sync = false;
    DM_types = 1;
    memset(myDM_list, 0, sizeof(myDM_list));
    memset(DM_school_number, 0, sizeof(DM_school_number));
}

void DMListSync() {
    DM_page = 1;
    dm_search_result = dm_messages->search_message(dm_messages, DM_school_number, DM_types);
    dm_search_result_length = dm_list_length(dm_search_result);
}