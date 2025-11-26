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
LostItem *viewItem;
char viewSchoolNumber[MAX_USERNAME_LEN];
bool viewSync = false;
int tagCount;
char totalTag[300];

extern char *tagNames[6];

void ViewScreen() {
    // title
    draw_text(viewItem->title, WIDTH / 2, 30, 70, TOP_CENTER, BLACK);

    // writer
    draw_text(TextFormat("Writer: %s", viewItem->writer), WIDTH - 50, 110, 45, TOP_RIGHT, BLACK);

    // content
    DrawTextBoxed(GetFontDefault(), viewItem->content, (Rectangle){.x = 50, .y = 240, .width = WIDTH - 100, .height = HEIGHT - 240 - 200}, 40, 1, true, BLACK);
    
    // NewText(TextFormat("Tag: "));
    draw_text(TextFormat("Target User: %s", (viewItem->target_user ? viewItem->target_user : "None")), 50, 160, 45, TOP_LEFT, BLACK);
    
    // Tags
    draw_text(TextFormat("Tags: %s", (strlen(totalTag) == 0 ? "None" : totalTag)), 50, HEIGHT - 180, 50, TOP_LEFT, BLACK);

    // go back to lobby
    draw_rectangle(50, HEIGHT - 100, WIDTH / 2 - 75, 60, TOP_LEFT, BLACK);
    draw_text("Back (B)", WIDTH / 4 + 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
    
    // Go to dm or Mark as returned(delete)
    draw_rectangle(WIDTH / 2 + 25, HEIGHT - 100, WIDTH / 2 - 75, 60, TOP_LEFT, BLACK);
    if (strcmp(viewItem->writer, viewSchoolNumber) == 0)
        draw_text("Mark as Deleted (M)", 3 * WIDTH / 4 - 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
    else 
        draw_text(TextFormat("DM to %s (D)", viewItem->writer), 3 * WIDTH / 4 - 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
}

void ViewReset() {
    view_item_id = -1;
    viewSync = false;
    tagCount = 0;
    totalTag[0] = '\0';
}

void ViewSynchronize() {
    tagCount = 0;
    totalTag[0] = '\0';
    for (int i = 0; i < 6; i++) {
        if (viewItem->tags[i]) {
            if (tagCount == 0) {
                strcat(totalTag, tagNames[i]);
            } else {
                strcat(totalTag, ", ");
                strcat(totalTag, tagNames[i]);
            }
            tagCount++;
        }
    }
}