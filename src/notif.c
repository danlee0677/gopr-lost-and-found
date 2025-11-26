#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/notif.h"

#define MAX_LINES 1024
#define FILE_NAME_MAX_LEN 100
#define LINES_PER_PAGE 5

extern char school_number[50];

int notif_sellected = 1;
char notif_lines[MAX_LINES][MAX_CONTENT_LEN];
int notif_count = 0;
int notif_total_page = 1;
int current_page = 1;
int from_notif = 0;
const char filename[FILE_NAME_MAX_LEN] = "./data/notif.txt";
char id[LINES_PER_PAGE][3][20];
char title[LINES_PER_PAGE][MAX_TITLE_LEN];
char content[LINES_PER_PAGE][MAX_LINES];
bool notif_to_DM = false;

void notif_screen() {
    draw_rectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    draw_text("Notifications", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);
    draw_rectangle(50, 200, WIDTH / 2 + 570, HEIGHT - 300, TOP_LEFT, BLACK);

    draw_rectangle(WIDTH - 50, 200, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Clear\n All (C)", WIDTH - 175, 235, 50, MIDDLE_CENTER, BLACK);
    draw_rectangle(WIDTH - 50, 360, 250, 120, TOP_RIGHT, BLACK);
    draw_text("Back (B)", WIDTH - 175, 420, 50, MIDDLE_CENTER, BLACK);

    float lineGap = (HEIGHT - 290) / 5;

    if (notif_count == 0) {
        draw_text("No Notifications", WIDTH / 2, HEIGHT / 2, 50, MIDDLE_CENTER, DARKGRAY);
        return;
    }

    for (int i = 0; i < LINES_PER_PAGE; i++) {
        int idx = (current_page - 1) * 5 + i;
        if (idx >= notif_count) break;

        draw_rectangle(50, 195 + lineGap * i + lineGap / 2, WIDTH / 2 + 570, lineGap - 10, MIDDLE_LEFT, BLACK);
        draw_text(TextFormat("(%d) %s", i + 1, id[i][0]), 70, 175 + lineGap * i + lineGap / 2, 40, MIDDLE_LEFT, BLACK);
        draw_text(TextFormat("%s", title[i]), 70, 175 + lineGap * i + lineGap / 2 + 40, 40, MIDDLE_LEFT, DARKGRAY);
    }

    draw_text("<-         ->", WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    draw_text(TextFormat("%d / %d", current_page, notif_total_page), WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
}

void read_notif_file() {
    notif_count = 0;
    FILE *nf = fopen("./data/notif.txt", "r");

    if (nf == NULL) {
        return;
    }

    while (fgets(notif_lines[notif_count], sizeof(notif_lines[notif_count]), nf) != NULL) {
        size_t len = strlen(notif_lines[notif_count]);
        if (len > 0 && (notif_lines[notif_count][len - 1] == '\n' || notif_lines[notif_count][len - 1] == '\r')) {
            notif_lines[notif_count][len - 1] = '\0';
        }

        sscanf(notif_lines[notif_count], "%s %s %s", id[0][0], id[0][1], id[0][2]);
        printf("%s %s %d\n", school_number, id[0][1], strcmp(id[0][1], school_number));

        if (strcmp(id[0][1], school_number) == 0)
            notif_count++;
    }
    fclose(nf);

    FILE *df;
    for (int i = 0; i < LINES_PER_PAGE; i++) {
        int idx = (current_page - 1) * LINES_PER_PAGE + i;
        if (idx >= notif_count) break;

        sscanf(notif_lines[idx], "%s %s %s", id[i][0], id[i][1], id[i][2]);
        printf("id[%d]: %s %s %s\n", i, id[i][0], id[i][1], id[i][2]);

        df = fopen(TextFormat("./data/dm/%s %s %s.txt", id[i][0], id[i][1], id[i][2]), "r");
        fgets(title[i], sizeof(title[i]), df);
        title[i][strcspn(title[i], "\r\n")] = '\0';
        fgets(content[i], sizeof(content[i]), df);
        content[i][strcspn(content[i], "\r\n")] = '\0';
    }

    fclose(df);

    if (notif_count > 0) {
        notif_total_page = (notif_count + (LINES_PER_PAGE - 1)) / LINES_PER_PAGE;
    } else {
        notif_total_page = 1;
    }
}

void clear_notif() {
    notif_count = 0;
    notif_total_page = 1;
    current_page = 1;
    FILE *f = fopen(filename, "w");
    fclose(f);
}

void notif_reset() {
    notif_sellected = 1;
    notif_count = 0;
    notif_total_page = 1;
    current_page = 1;
    memset(notif_lines, 0, sizeof(notif_lines));
    memset(id, 0, sizeof(id));
    memset(title, 0, sizeof(title));
    memset(content, 0, sizeof(content));
}
