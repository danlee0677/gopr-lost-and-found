#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/draw.h"
#include "../include/constants.h"
#include "../include/models.h"
#include "../include/textbox.h"
#include "../include/view.h"

extern LostItemList *lost_items;
int view_item_id;
LostItem *view_item;
char view_school_number[MAX_USERNAME_LEN];
bool view_sync = false;
int tag_count;
char total_tag[300];

extern char *tag_names[6];

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
    draw_text(TextFormat("Tags: %s", (strlen(total_tag) == 0 ? "None" : total_tag)), 50, HEIGHT - 180, 50, TOP_LEFT, BLACK);

    // go back to lobby
    draw_rectangle(50, HEIGHT - 100, WIDTH / 2 - 75, 60, TOP_LEFT, BLACK);
    draw_text("Back (B)", WIDTH / 4 + 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
    
    // Go to dm or Mark as returned(delete)
    draw_rectangle(WIDTH / 2 + 25, HEIGHT - 100, WIDTH / 2 - 75, 60, TOP_LEFT, BLACK);
    if (strcmp(view_item->writer, view_school_number) == 0)
        draw_text("Mark as Deleted (M)", 3 * WIDTH / 4 - 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
    else 
        draw_text(TextFormat("DM to %s (D)", view_item->writer), 3 * WIDTH / 4 - 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
}

void view_reset() {
    view_item_id = -1;
    view_sync = false;
    tag_count = 0;
    total_tag[0] = '\0';
}

void view_synchronize() {
    tag_count = 0;
    total_tag[0] = '\0';
    for (int i = 0; i < 6; i++) {
        if (view_item->tags[i]) {
            if (tag_count == 0) {
                strcat(total_tag, tag_names[i]);
            } else {
                strcat(total_tag, ", ");
                strcat(total_tag, tag_names[i]);
            }
            tag_count++;
        }
    }
}