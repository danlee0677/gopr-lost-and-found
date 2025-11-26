/*
view.c: 분실물 보는 기능
2025-11-26 이승준 전역변수 앞에 g_, 지역변수 앞에 s_, 함수명, 변수명 snake case로 변형
2025-11-21 이승준 로비로 돌아가기, 글쓴이에게 DM 보내기 추가
2025-11-20 이승준 분실물 게시글 보는 기능 추가
*/
#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/draw.h"
#include "../include/constants.h"
#include "../include/models.h"
#include "../include/textbox.h"
#include "../include/view.h"

extern LostItemList *g_lost_items; // 분실물 담는 동적 배열 포인터
int g_view_item_id; // 현재 보는 분실물 게시글 id
LostItem *view_item; // 현재 띄워야 하는 분실물 게시글
char g_view_school_number[MAX_USERNAME_LEN];
bool g_view_sync = false;
int g_tag_count; // 태그 개수
char g_total_tag[300]; // 태그 전체를 char[]로 포멧팅

extern char *g_tag_names[6]; // 태그 이름들

// 분실물 게시글 보는 페이지
void view_screen() {
    // title
    draw_text(view_item->title, WIDTH / 2, 30, 70, TOP_CENTER, BLACK);

    // writer
    draw_text(TextFormat("Writer: %s", view_item->writer), WIDTH - 50, 110, 45, TOP_RIGHT, BLACK);

    // content
    DrawTextBoxed(GetFontDefault(), view_item->content, (Rectangle){.x = 50, .y = 240, .width = WIDTH - 100, .height = HEIGHT - 240 - 200}, 40, 1, true, BLACK);
    
    // NewText(TextFormat("Tag: "));
    draw_text(TextFormat("Target User: %s", (view_item->target_user ? view_item->target_user : "None")), 50, 160, 45, TOP_LEFT, BLACK);
    
    // Tags
    draw_text(TextFormat("Tags: %s", (strlen(g_total_tag) == 0 ? "None" : g_total_tag)), 50, HEIGHT - 180, 50, TOP_LEFT, BLACK);

    // go back to lobby
    draw_rectangle(50, HEIGHT - 100, WIDTH / 2 - 75, 60, TOP_LEFT, BLACK);
    draw_text("Back (B)", WIDTH / 4 + 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
    
    // Go to dm or Mark as returned(delete)
    draw_rectangle(WIDTH / 2 + 25, HEIGHT - 100, WIDTH / 2 - 75, 60, TOP_LEFT, BLACK);
    if (strcmp(view_item->writer, g_view_school_number) == 0)
        draw_text("Mark as Deleted (M)", 3 * WIDTH / 4 - 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
    else 
        draw_text(TextFormat("DM to %s (D)", view_item->writer), 3 * WIDTH / 4 - 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
}

// 게시글 보는것 리셋
void view_reset() {
    g_view_item_id = -1;
    g_view_sync = false;
    g_tag_count = 0;
    g_total_tag[0] = '\0';
}

// 분실물 게시글 리스트 동기화
void view_synchronize() {
    g_tag_count = 0;
    g_total_tag[0] = '\0';
    for (int i = 0; i < 6; i++) {
        if (view_item->tags[i]) {
            if (g_tag_count == 0) {
                strcat(g_total_tag, g_tag_names[i]);
            } else {
                strcat(g_total_tag, ", ");
                strcat(g_total_tag, g_tag_names[i]);
            }
            g_tag_count++;
        }
    }
}