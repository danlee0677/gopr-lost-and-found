#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/notif.h"

extern char g_school_number[50];

int g_notif_sellected = 1;
char g_notif_lines[MAX_LINES][MAX_CONTENT_LEN];
int g_notif_count = 0;
int g_notif_total_page = 1;
int g_current_page = 1;
int g_from_notif = 0;
const char g_notif_filename[FILE_NAME_MAX_LEN] = "./data/notif.txt";
char g_id[LINES_PER_PAGE][3][20];
char g_title[LINES_PER_PAGE][MAX_TITLE_LEN];
char g_notif_content[LINES_PER_PAGE][MAX_LINES];
bool g_notif_to_DM = false;

void notif_screen() {
    draw_rectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    draw_text("Notifications", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);
    draw_rectangle(50, 200, WIDTH / 2 + 570, HEIGHT - 300, TOP_LEFT, BLACK);

    draw_rectangle(WIDTH - 50, 200, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Clear\n All (C)", WIDTH - 175, 235, 50, MIDDLE_CENTER, BLACK);
    draw_rectangle(WIDTH - 50, 360, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Back (B)", WIDTH - 175, 420, 50, MIDDLE_CENTER, BLACK);

    float line_gap = (HEIGHT - 290) / 5;

    if (g_notif_count == 0) {
        draw_text("No Notifications", WIDTH / 2, HEIGHT / 2, 50, MIDDLE_CENTER, DARKGRAY);
        return;
    }

    for (int i = 0; i < LINES_PER_PAGE; i++) {
        int idx = (g_current_page - 1) * 5 + i;
        if (idx >= g_notif_count) break;

        draw_rectangle(50, 195 + line_gap * i + line_gap / 2, WIDTH / 2 + 570, line_gap - 10, MIDDLE_LEFT, BLACK);
        draw_text(TextFormat("(%d) %s", i + 1, g_id[i][0]), 70, 175 + line_gap * i + line_gap / 2, 40, MIDDLE_LEFT, BLACK);
        draw_text(TextFormat("%s", g_title[i]), 70, 175 + line_gap * i + line_gap / 2 + 40, 40, MIDDLE_LEFT, DARKGRAY);
    }

    draw_text("<-         ->", WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    draw_text(TextFormat("%d / %d", g_current_page, g_notif_total_page), WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
}

void read_notif_file() {
    g_notif_count = 0;
    FILE *nf = fopen("./data/notif.txt", "r");

    if (nf == NULL) {
        return;
    }

    while (fgets(g_notif_lines[g_notif_count], sizeof(g_notif_lines[g_notif_count]), nf) != NULL) {
        size_t len = strlen(g_notif_lines[g_notif_count]);
        if (len > 0 && (g_notif_lines[g_notif_count][len - 1] == '\n' || g_notif_lines[g_notif_count][len - 1] == '\r')) {
            g_notif_lines[g_notif_count][len - 1] = '\0';
        }

        sscanf(g_notif_lines[g_notif_count], "%s %s %s", g_id[0][0], g_id[0][1], g_id[0][2]);
        printf("%s %s %d\n", g_school_number, g_id[0][1], strcmp(g_id[0][1], g_school_number));

        if (strcmp(g_id[0][1], g_school_number) == 0)
            g_notif_count++;
    }
    fclose(nf);

    FILE *df;
    for (int i = 0; i < LINES_PER_PAGE; i++) {
        int idx = (g_current_page - 1) * LINES_PER_PAGE + i;
        if (idx >= g_notif_count) break;

        sscanf(g_notif_lines[idx], "%s %s %s", g_id[i][0], g_id[i][1], g_id[i][2]);
        printf("id[%d]: %s %s %s\n", i, g_id[i][0], g_id[i][1], g_id[i][2]);

        df = fopen(TextFormat("./data/dm/%s %s %s.txt", g_id[i][0], g_id[i][1], g_id[i][2]), "r");
        fgets(g_title[i], sizeof(g_title[i]), df);
        g_title[i][strcspn(g_title[i], "\r\n")] = '\0';
        fgets(g_notif_content[i], sizeof(g_notif_content[i]), df);
        g_notif_content[i][strcspn(g_notif_content[i], "\r\n")] = '\0';
    }

    fclose(df);

    if (g_notif_count > 0) {
        g_notif_total_page = (g_notif_count + (LINES_PER_PAGE - 1)) / LINES_PER_PAGE;
    } else {
        g_notif_total_page = 1;
    }
}

void clear_notif() {
    g_notif_count = 0;
    g_notif_total_page = 1;
    g_current_page = 1;
    FILE *f = fopen(g_notif_filename, "w");
    fclose(f);
}

void notif_reset() {
    g_notif_sellected = 1;
    g_notif_count = 0;
    g_notif_total_page = 1;
    g_current_page = 1;
    memset(g_notif_lines, 0, sizeof(g_notif_lines));
    memset(g_id, 0, sizeof(g_id));
    memset(g_title, 0, sizeof(g_title));
    memset(g_notif_content, 0, sizeof(g_notif_content));
}
