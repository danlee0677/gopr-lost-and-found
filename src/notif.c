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

char notif_lines[100][256];
int notif_count = 0;
int notif_total_page = 1;
int current_page = 1;

void NotifScreen() {
    NewRectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    NewText("Notifications", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(50, 200, WIDTH - 100, HEIGHT - 300, TOP_LEFT, BLACK);
    
    readNotifFile();

    float lineGap = (HEIGHT - 290) / 5;

    if(notif_count == 0) {
        NewText("No new notifications", WIDTH / 2, HEIGHT / 2, 50, MIDDLE_CENTER, LIGHTGRAY);
        return;
    }

    for (int i = 0; i < 5; i++) {
        int idx = (current_page - 1) * 5 + i;
        if (idx >= notif_count) break;
        NewRectangle(50, 195 + lineGap * i + lineGap / 2, WIDTH - 100, lineGap - 10, MIDDLE_LEFT, BLACK);
        NewText(TextFormat("(%d) %s", i + 1, notif_lines[idx]), 70, 195 + lineGap * i + lineGap / 2, 40, MIDDLE_LEFT, BLACK);
    }

    char pageInfo[64];
    
    NewText("<-         ->", WIDTH / 2, HEIGHT - 60, 50, MIDDLE_CENTER, BLACK);
    NewText(TextFormat("%d / %d", current_page, notif_total_page), WIDTH / 2, HEIGHT - 60, 50, MIDDLE_CENTER, BLACK);
}

void readNotifFile() {
    notif_count = 0;
    FILE *f=fopen("../data/notif.txt", "r");

    if (f==NULL) {
        NewText("No new notifications", WIDTH / 2, HEIGHT / 2, 50, MIDDLE_CENTER, LIGHTGRAY);
        return;
    }
    while (fgets(notif_lines[notif_count], sizeof(notif_lines[notif_count]), f)!=NULL){
        size_t len = strlen(notif_lines[notif_count]);
        if (len > 0 && (notif_lines[notif_count][len - 1] == '\n' || notif_lines[notif_count][len - 1] == '\r')) {
            notif_lines[notif_count][len - 1] = '\0';
        }
        notif_count++;
    }
    fclose(f);
    if (notif_count > 0) {
    notif_total_page = (notif_count + 4) / 5;
    } else {
        notif_total_page = 1;
    }
}

void clearNotif() {
    remove("../data/notif.txt");
    notif_count = 0;
    notif_total_page = 1;
    current_page = 1;
    FILE *f=fopen("../data/notif.txt", "w");
    fclose(f);
}