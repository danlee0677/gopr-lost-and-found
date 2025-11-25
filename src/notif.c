#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/notif.h"

#define MAX_LINES 1024
#define LINES_PER_PAGE 5

extern char schoolNumber[50];

int notif_sellected = 1;
char notif_lines[100][256];
int notif_count = 0;
int notif_total_page = 1;
int current_page = 1;
int fromNotif = 0;
const char filename[30] = "./data/notif.txt";
char id[5][3][20];
char title[5][250];
char content[5][1000];
bool notifToDM = false;

void NotifScreen() {
    NewRectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    NewText("Notifications", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(50, 200, WIDTH / 2 + 570, HEIGHT - 300, TOP_LEFT, BLACK);

    NewRectangle(WIDTH - 50, 200, 250, 120, TOP_RIGHT, BLACK);
    NewText("Clear\n All (C)", WIDTH - 175, 235, 50, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH - 50, 360, 250, 120, TOP_RIGHT, BLACK);
    NewText("Back (B)", WIDTH - 175, 420, 50, MIDDLE_CENTER, BLACK);

    float lineGap = (HEIGHT - 290) / 5;

    if (notif_count == 0) {
        NewText("No Notifications", WIDTH / 2, HEIGHT / 2, 50, MIDDLE_CENTER, DARKGRAY);
        return;
    }

    for (int i = 0; i < 5; i++) {
        int idx = (current_page - 1) * 5 + i;
        if (idx >= notif_count) break;

        NewRectangle(50, 195 + lineGap * i + lineGap / 2, WIDTH / 2 + 570, lineGap - 10, MIDDLE_LEFT, BLACK);
        NewText(TextFormat("(%d) %s", i + 1, id[i][0]), 70, 175 + lineGap * i + lineGap / 2, 40, MIDDLE_LEFT, BLACK);
        NewText(TextFormat("%s", title[i]), 70, 175 + lineGap * i + lineGap / 2 + 40, 40, MIDDLE_LEFT, DARKGRAY);
    }

    NewText("<-         ->", WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    NewText(TextFormat("%d / %d", current_page, notif_total_page), WIDTH / 4 + 300, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
}

void readNotifFile() {
    notif_count = 0;
    FILE *f = fopen("./data/notif.txt", "r");

    if (f == NULL) {
        return;
    }

    while (fgets(notif_lines[notif_count], sizeof(notif_lines[notif_count]), f) != NULL) {
        size_t len = strlen(notif_lines[notif_count]);
        if (len > 0 && (notif_lines[notif_count][len - 1] == '\n' || notif_lines[notif_count][len - 1] == '\r')) {
            notif_lines[notif_count][len - 1] = '\0';
        }

        sscanf(notif_lines[notif_count], "%s %s %s", id[0][0], id[0][1], id[0][2]);
        printf("%s %s %d\n", schoolNumber, id[0][1], strcmp(id[0][1], schoolNumber));

        if (strcmp(id[0][1], schoolNumber) == 0)
            notif_count++;
    }

    for (int i = 0; i < 5; i++) {
        int idx = (current_page - 1) * 5 + i;
        if (idx >= notif_count) break;

        sscanf(notif_lines[idx], "%s %s %s", id[i][0], id[i][1], id[i][2]);
        printf("id[%d]: %s %s %s\n", i, id[i][0], id[i][1], id[i][2]);

        f = fopen(TextFormat("./data/dm/%s %s %s.txt", id[i][0], id[i][1], id[i][2]), "r");
        fgets(title[i], sizeof(title[i]), f);
        title[i][strcspn(title[i], "\r\n")] = '\0';
        fgets(content[i], sizeof(content[i]), f);
        content[i][strcspn(content[i], "\r\n")] = '\0';
    }

    fclose(f);

    if (notif_count > 0) {
        notif_total_page = (notif_count + 4) / 5;
    } else {
        notif_total_page = 1;
    }
}

void clearNotif() {
    remove(filename);
    notif_count = 0;
    notif_total_page = 1;
    current_page = 1;
    FILE *f = fopen(filename, "w");
    fclose(f);
}

void notifReset() {
    notif_sellected = 1;
    notif_count = 0;
    notif_total_page = 1;
    current_page = 1;
    memset(notif_lines, 0, sizeof(notif_lines));
    memset(id, 0, sizeof(id));
    memset(title, 0, sizeof(title));
    memset(content, 0, sizeof(content));
}
