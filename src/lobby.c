#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/models.h"
#include "../include/draw.h"
#include "../include/lobby.h"

char lobbySchoolNumber[50];
char lobbySearch[1000];
int lobbySelected = 0;
int lobbyPage = 0;
int lobbyTags = 0;
bool lobbyTagsSelected[6] = {false, false, false, false, false, false};

void LobbyScreen() {
    NewRectangle(50, 50, WIDTH / 2 + 200, 100, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 + 300, 50, 250, 100, TOP_LEFT, BLACK);
    NewText(TextFormat("Hello, %s!", lobbySchoolNumber), WIDTH - 50, 75, 50, TOP_RIGHT, BLACK);
    NewRectangle(50, 200, WIDTH / 2 + 200, HEIGHT - 300, TOP_LEFT, BLACK);
    NewText(TextFormat("<-  %d  ->", lobbyPage), WIDTH / 4 + 125, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    
    if (lobbySelected != 1 && strlen(lobbySearch) == 0) NewText("Press (S) to search", 70, 75, 50, TOP_LEFT, GRAY);
    else NewText(lobbySearch, 70, 75, 50, TOP_LEFT, BLACK);
    
    NewText("Tags (T)", WIDTH / 2 + 425, 75, 50, TOP_CENTER, BLACK);
    NewRectangle(WIDTH / 2 + 300, 200, 320, HEIGHT - 300, TOP_LEFT, BLACK);
    
    NewRectangle(WIDTH - 50, 200, 250, 150, TOP_RIGHT, BLACK);
    NewRectangle(WIDTH - 50, 390, 250, 150, TOP_RIGHT, BLACK);
    NewRectangle(WIDTH - 50, 580, 250, 150, TOP_RIGHT, BLACK);

    NewText("Post (P)", WIDTH - 175, 275, 50, MIDDLE_CENTER, BLACK);
    NewText("DM (D)", WIDTH - 175, 465, 50, MIDDLE_CENTER, BLACK);
    NewText("Logout (L)", WIDTH - 175, 655, 40, MIDDLE_CENTER, BLACK);

    NewText("Electronics", WIDTH / 2 + 320, 225, 35, TOP_LEFT, BLACK);
    NewText("Clothings", WIDTH / 2 + 320, 315, 35, TOP_LEFT, BLACK);
    NewText("Bags", WIDTH / 2 + 320, 405, 35, TOP_LEFT, BLACK);
    NewText("Stationery", WIDTH / 2 + 320, 495, 35, TOP_LEFT, BLACK);
    NewText("Personal", WIDTH / 2 + 320, 585, 35, TOP_LEFT, BLACK);
    NewText("Special", WIDTH / 2 + 320, 675, 35, TOP_LEFT, BLACK);

    NewRectangle(WIDTH / 2 + 550, 220, 50, 50, TOP_LEFT, (lobbyTags == 0) ? RED : BLACK);
    NewRectangle(WIDTH / 2 + 550, 310, 50, 50, TOP_LEFT, (lobbyTags == 1) ? RED : BLACK);
    NewRectangle(WIDTH / 2 + 550, 400, 50, 50, TOP_LEFT, (lobbyTags == 2) ? RED : BLACK);
    NewRectangle(WIDTH / 2 + 550, 490, 50, 50, TOP_LEFT, (lobbyTags == 3) ? RED : BLACK);
    NewRectangle(WIDTH / 2 + 550, 580, 50, 50, TOP_LEFT, (lobbyTags == 4) ? RED : BLACK);
    NewRectangle(WIDTH / 2 + 550, 670, 50, 50, TOP_LEFT, (lobbyTags == 5) ? RED : BLACK);
}