#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/dm.h"
#include "../include/dmview.h"
#include "../include/models.h"



int g_viewDM_id;

extern DMList *g_dm_messages;
DMMessage *g_viewDM;

void DMview_screen() {
    // title
    draw_text(g_viewDM->title, WIDTH / 2, 30, 70, TOP_CENTER, BLACK);

    // sender
    draw_text(TextFormat("Writer: %s", g_viewDM->sender), WIDTH - 50, 110, 45, TOP_RIGHT, BLACK);

    // content
    DrawTextBoxed(GetFontDefault(), g_viewDM->content, (Rectangle){.x = 50, .y = 240, .width = WIDTH - 100, .height = HEIGHT - 240 - 200}, 40, 1, true, BLACK);

    // go back to lobby
    draw_rectangle(50, HEIGHT - 100, WIDTH / 2 - 75, 60, TOP_LEFT, BLACK);
    draw_text("Back (B)", WIDTH / 4 + 12.5, HEIGHT - 70, 40, MIDDLE_CENTER, BLACK);
}

void DMview_reset() {
}