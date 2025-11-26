#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/textbox.h"
#include "../include/models.h"
#include "../include/draw.h"
#include "../include/lobby.h"

char lobby_school_number[50];
char lobby_search[MAX_SEARCH_LEN];
int lobby_selected = 0;
int lobby_page = 1;
bool lobby_tags_selected[6] = {false, false, false, false, false, false};
extern LostItemList* lost_items;
int *lobby_search_result = NULL;
int lobby_search_result_length = 0;
char lobby_dummy_user[1] = "";
bool lobby_target_user_selected = false;
bool lobby_sync = false; // for syncing the lobby, only at first
bool lobby_filters[2] = {false, false};

/*
lobbyselected:
1: Search bar
2: Tag
*/

const char *tag_names[6] = {"Electronics", "Clothings", "Bags", "Stationery", "Personal", "Special"};
const char *tag_numbers[6] = {"1", "2", "3", "4", "5", "6"};
const char *filter_names[2] = {"A", "W"};

void lobby_screen() {
    // Search bar
    draw_rectangle(50, 50, WIDTH / 2 + 200, 100, TOP_LEFT, (lobby_selected == 1 ? RED : BLACK));
    if (lobby_selected != 1 && strlen(lobby_search) == 0) draw_text("Press (S) to search", 70, 75, 50, TOP_LEFT, GRAY);
    else draw_text(lobby_search, 70, 75, 50, TOP_LEFT, BLACK);
    
    // Tag box
    draw_rectangle(WIDTH / 2 + 300, 50, 250, 100, TOP_LEFT, (lobby_selected == 2 ? RED : BLACK));
    draw_text("Tags (T)", WIDTH / 2 + 425, 75, 50, TOP_CENTER, BLACK);

    // tags
    draw_rectangle(WIDTH / 2 + 300, 200, 320, HEIGHT - 450, TOP_LEFT, BLACK);
    for (int i = 0; i < 6; i++) {
        draw_rectangle(WIDTH / 2 + 550, 220 + 90 * i, 50, 50, TOP_LEFT, BLACK);
        if (lobby_tags_selected[i]) {
            DrawRectangle(WIDTH / 2 + 550, 220 + 90 * i, 50, 50, BLACK);
        }
        draw_text(tag_names[i], WIDTH / 2 + 320, 225 + 90 * i, 35, TOP_LEFT, BLACK);
        if (lobby_selected == 2) {
            draw_text(tag_numbers[i], WIDTH / 2 + 575, 245 + 90 * i, 35, MIDDLE_CENTER, (lobby_tags_selected[i] ? WHITE : BLACK));
        }
    }
    
    // clear
    draw_text("Clear (C)", WIDTH / 2 + 460, HEIGHT - 155, 50, MIDDLE_CENTER, BLACK);
    draw_rectangle(WIDTH / 2 + 300, HEIGHT - 210, 320, 110, TOP_LEFT, BLACK);

    // top right 
    draw_text(TextFormat("Hello, %s!", lobby_school_number), WIDTH - 50, 75, 50, TOP_RIGHT, BLACK);

    // Search Result Display
    draw_rectangle(50, 200, WIDTH / 2 + 200, HEIGHT - 300, TOP_LEFT, BLACK);
    draw_text("<-         ->", WIDTH / 4 + 125, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    int denominator = lobby_search_result_length / 5 + (lobby_search_result_length % 5 ? 1 : 0);
    draw_text(TextFormat("%d / %d", lobby_page, (denominator ? denominator : 1)), WIDTH / 4 + 125, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    
    int result_height = (HEIGHT - 290) / 5;
    for (int i = 0; i < 5; i++) {
        int ind = (lobby_page - 1) * 5 + i;
        if (ind < lobby_search_result_length) {
            draw_rectangle(50, 195 + result_height * i + result_height / 2, WIDTH / 2 + 200, result_height - 10, MIDDLE_LEFT, BLACK);
            draw_text(TextFormat("(%d) %s", i + 1, lost_items->list[lobby_search_result[ind]]->title), 70, 195 + result_height * i + result_height * 0.3, 50, MIDDLE_LEFT, BLACK);
            draw_text(TextFormat("%.50s...", lost_items->list[lobby_search_result[ind]]->content), 70, 195 + result_height * i + result_height * 0.7, 30, MIDDLE_LEFT, BLACK);
        }
    }

    // Post    
    draw_rectangle(WIDTH - 50, 200, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Post (P)", WIDTH - 175, 260, 50, MIDDLE_CENTER, BLACK);

    // DM
    draw_rectangle(WIDTH - 50, 360, 250, 120, TOP_RIGHT, BLACK);
    draw_text("DM (D)", WIDTH - 175, 420, 50, MIDDLE_CENTER, BLACK);

    // Logout
    draw_rectangle(WIDTH - 50, 520, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Logout (L)", WIDTH - 175, 580, 40, MIDDLE_CENTER, BLACK);

    // Filter
    draw_rectangle(WIDTH - 50, 680, 250, HEIGHT - 100 - 680, TOP_RIGHT, BLACK);
    draw_text("Filter", WIDTH - 175, 680 + 25, 40, MIDDLE_CENTER, BLACK);
    draw_text(TextFormat("Address: %s", lobby_school_number), WIDTH - 290, 680 + 90, 23, MIDDLE_LEFT, BLACK);
    draw_rectangle(WIDTH - 65, 680 + 90, 30, 30, MIDDLE_RIGHT, BLACK);
    
    draw_text(TextFormat("Writer: %s", lobby_school_number), WIDTH - 290, 680 + 150, 23, MIDDLE_LEFT, BLACK);
    draw_rectangle(WIDTH - 65, 680 + 150, 30, 30, MIDDLE_RIGHT, BLACK);

    for (int i = 0; i < 2; i++) {
        if (lobby_filters[i]) DrawRectangle(WIDTH - 95, 680 + 75 + 60 * i, 30, 30, BLACK);
        draw_text(filter_names[i], WIDTH - 80, 680 + 90 + 60 * i, 20, MIDDLE_CENTER, (lobby_filters[i] ? WHITE : BLACK));
    }

}

void lobby_reset() {
    if (lobby_search_result) {
        free(lobby_search_result);
        lobby_search_result = NULL;
    }
    lobby_sync = false;
    memset(lobby_school_number, '\0', 50);
    memset(lobby_search, '\0', MAX_SEARCH_LEN);
    lobby_selected = 0;
    lobby_page = 1;
    memset(lobby_tags_selected, false, 6);
    memset(lobby_filters, false, 2);
    lobby_search_result_length = 0;
    lobby_target_user_selected = false;
}

void lobby_sync_lost_item_list() {
    lobby_page = 1;
    lobby_search_result = lost_items->search_lost_item(lost_items, lobby_search, lobby_tags_selected, lobby_school_number, lobby_filters);
    lobby_search_result_length = lost_item_list_length(lobby_search_result);
}