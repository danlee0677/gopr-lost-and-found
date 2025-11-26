#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/textbox.h"
#include "../include/dmsend.h"

char g_DMsend_title[MAX_TITLE_LEN];
char g_DMsend_content[MAX_CONTENT_LEN];
char g_DMsend_receiver[MAX_USERNAME_LEN];
char g_DMsend_school_number[MAX_USERNAME_LEN];
const char *g_DMsend_error_msg[] = {
    "",
    "Invalid Title",
    "Invalid Content",
    "Invalid Target User",
};
int g_DMsend_selected = 0;
int g_DMsend_error = 0;

void DMsend_screen() {
    // error msg
    if (g_DMsend_error) draw_text(g_DMsend_error_msg[g_DMsend_error], WIDTH - 50, 30, 40, TOP_RIGHT, RED);
    
    // title
    draw_rectangle(50, 85, WIDTH - 100, 55, TOP_LEFT, (g_DMsend_selected == 1 ? RED : BLACK));
    draw_text(TextFormat("%s", g_DMsend_title), 57, 112.5, 40, MIDDLE_LEFT, BLACK);
    draw_text("Title (T)", 50, 30, 40, TOP_LEFT, BLACK);
    
    // content
    Rectangle text_box_area = {.x = 55, .y = 220, .width = WIDTH - 105, .height = HEIGHT - 360 - 220};
    draw_text("Content (C)", 50, 165, 40, TOP_LEFT, BLACK);
    draw_rectangle(50, 220, WIDTH - 100, HEIGHT - 360 - 220, TOP_LEFT, (g_DMsend_selected == 2 ? RED : BLACK));
    DrawTextBoxed(GetFontDefault(), g_DMsend_content, text_box_area, 40, 1, true, BLACK);

    // target_user
    // NewRectangle(50, 500, WIDTH - 100)
    draw_text("Receiver (U)", 50, HEIGHT - 335, 40, TOP_LEFT, BLACK);
    draw_rectangle(50, HEIGHT - 280, WIDTH - 100, 50, TOP_LEFT, (g_DMsend_selected == 3 ? RED : BLACK));
    draw_text(g_DMsend_receiver, 55, HEIGHT - 255, 40, MIDDLE_LEFT, BLACK);

    // post / go back button
    // post
    draw_rectangle(50, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    draw_text("Back (B)", (WIDTH / 2 + 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
    // back
    draw_rectangle(WIDTH / 2 + 25, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    draw_text("Post (P)", (3 * WIDTH / 2 - 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
}

void DMsend_reset() {
    memset(g_DMsend_title, '\0', MAX_TITLE_LEN);
    memset(g_DMsend_content, '\0', MAX_CONTENT_LEN);
    memset(g_DMsend_receiver, '\0', MAX_USERNAME_LEN);
    g_DMsend_error = 0;
    g_DMsend_selected = 0;
}

bool DMsend_valid() {
    if (strlen(g_DMsend_title) == 0) {
        g_DMsend_error = 1;
        return false;
    } else if (strlen(g_DMsend_content) == 0) {
        g_DMsend_error = 2;
        return false;
    } else if (strlen(g_DMsend_receiver) != 0 && strlen(g_DMsend_receiver) != 5) {
        g_DMsend_error = 3;
        return false;
    }
    g_DMsend_error = true;
    return true;
}