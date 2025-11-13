#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/lobby.h"

char lobbySchoolNumber[50];
int lobbySelected = 0;
int lobbyPage = 0;
int lobbyTags = 0;

void LobbyScreen() {
    NewRectangle(50, 50, WIDTH / 2 + 200, 100, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 + 300, 50, 250, 100, TOP_LEFT, BLACK);
    NewText(TextFormat("Hello, %s!", lobbySchoolNumber), WIDTH - 50, 75, 50, TOP_RIGHT, BLACK);
    NewRectangle(50, 200, WIDTH / 2 + 200, HEIGHT - 300, TOP_LEFT, BLACK);
    NewText(TextFormat("<-  %d  ->", lobbyPage), WIDTH / 4 + 125, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    NewText("Press (S) to search", 70, 75, 50, TOP_LEFT, GRAY);
    NewText("Tags (T)", WIDTH / 2 + 425, 75, 50, TOP_CENTER, BLACK);
    NewRectangle(WIDTH / 2 + 300, 200, 250, HEIGHT - 300, TOP_LEFT, BLACK);
    NewRectangle(WIDTH - 50, 200, 320, 150, TOP_RIGHT, BLACK);
    NewRectangle(WIDTH - 50, 390, 320, 150, TOP_RIGHT, BLACK);
    NewRectangle(WIDTH - 50, 580, 320, 150, TOP_RIGHT, BLACK);
    NewText("Post (P)", WIDTH - 210, 275, 50, MIDDLE_CENTER, BLACK);
    NewText("DM (D)", WIDTH - 210, 465, 50, MIDDLE_CENTER, BLACK);
    NewText("Logout (L)", WIDTH - 210, 655, 50, MIDDLE_CENTER, BLACK);
}