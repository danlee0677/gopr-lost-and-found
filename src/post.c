#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/textbox.h"
#include "../include/post.h"


char post_title[MAX_TITLE_LEN];
char post_content[MAX_CONTENT_LEN];
char post_target_user[MAX_USERNAME_LEN];
char post_school_number[MAX_USERNAME_LEN];
const char *post_error_msg[] = {
    "",
    "Invalid Title",
    "Invalid Content",
    "Invalid Target User",
};
int post_selected = 0;
int post_error = 0;
bool post_tags_selected[6] = {false, false, false, false, false, false};


void post_screen() {
    // error msg
    if (post_error) draw_text(post_error_msg[post_error], WIDTH - 50, 30, 40, TOP_RIGHT, RED);
    
    // title
    draw_rectangle(50, 85, WIDTH - 100, 55, TOP_LEFT, (post_selected == 1 ? RED : BLACK));
    draw_text(TextFormat("%s", post_title), 57, 112.5, 40, MIDDLE_LEFT, BLACK);
    draw_text("Title (T)", 50, 30, 40, TOP_LEFT, BLACK);
    
    // content
    Rectangle text_box_area = {.x = 55, .y = 220, .width = WIDTH - 105, .height = HEIGHT - 360 - 220};
    draw_text("Content (C)", 50, 165, 40, TOP_LEFT, BLACK);
    draw_rectangle(50, 220, WIDTH - 100, HEIGHT - 360 - 220, TOP_LEFT, (post_selected == 2 ? RED : BLACK));
    DrawTextBoxed(GetFontDefault(), post_content, text_box_area, 40, 1, true, BLACK);

    // target_user
    // NewRectangle(50, 500, WIDTH - 100)
    draw_text("Target User (U)", 50, HEIGHT - 335, 40, TOP_LEFT, BLACK);
    draw_rectangle(50, HEIGHT - 280, WIDTH - 100, 50, TOP_LEFT, (post_selected == 3 ? RED : BLACK));
    draw_text(post_target_user, 55, HEIGHT - 255, 40, MIDDLE_LEFT, BLACK);

    // tags
    draw_text("Tags", 50, HEIGHT - 215, 40, TOP_LEFT, BLACK);
    int tag_width = (WIDTH - 250) / 6;
    const char *tag_number_names[6] = {"Electronics [1]", "Clothings [2]", "Bags [3]", "Stationery [4]", "Personal [5]", "Special [6]"};
    for (int i = 0; i < 6; i++) {
        draw_rectangle(50 + (30 + tag_width) * i, HEIGHT - 160, tag_width, 50, TOP_LEFT, (post_tags_selected[i] ? RED : BLACK));
        draw_text(tag_number_names[i], 50 + (tag_width + 30) * i + tag_width / 2, HEIGHT - 135, 30, MIDDLE_CENTER, BLACK);
    }
    

    // post / go back button
    // post
    draw_rectangle(50, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    draw_text("Back (B)", (WIDTH / 2 + 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
    // back
    draw_rectangle(WIDTH / 2 + 25, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    draw_text("Post (P)", (3 * WIDTH / 2 - 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
}

void post_reset() {
    memset(post_title, '\0', MAX_TITLE_LEN);
    memset(post_content, '\0', MAX_CONTENT_LEN);
    memset(post_target_user, '\0', MAX_USERNAME_LEN);
    memset(post_tags_selected, false, 6);
    post_error = 0;
    post_selected = 0;
}

bool post_valid() {
    if (strlen(post_title) == 0) {
        post_error = 1;
        return false;
    } else if (strlen(post_content) == 0) {
        post_error = 2;
        return false;
    } else if (strlen(post_target_user) != 0 && strlen(post_target_user) != 5) {
        post_error = 3;
        return false;
    }
    post_error = true;
    return true;
}