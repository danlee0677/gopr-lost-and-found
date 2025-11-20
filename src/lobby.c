#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/models.h"
#include "../include/draw.h"
#include "../include/lobby.h"

char lobbySchoolNumber[50];
char lobbySearch[MAX_SEARCH_LEN];
int lobbySelected = 0;
int lobbyPage = 1;
bool lobbyTagsSelected[6] = {false, false, false, false, false, false};
extern LostItemList* lostItems;
int *lobbySearchResult;
int lobbySearchResultLength = 0;
char lobbyDummyUser[1] = "";
bool lobbyTargetUserSelected = false;
bool lobbySync = false; // for syncing the lobby, only at first

/*
lobbyselected:
1: Search bar
2: Tag
*/

const char *tagNames[6] = {"Electronics", "Clothings", "Bags", "Stationery", "Personal", "Special"};
const char *tagNumbers[6] = {"1", "2", "3", "4", "5", "6"};

void LobbyScreen() {
    // Search bar
    NewRectangle(50, 50, WIDTH / 2 + 200, 100, TOP_LEFT, (lobbySelected == 1 ? RED : BLACK));
    if (lobbySelected != 1 && strlen(lobbySearch) == 0) NewText("Press (S) to search", 70, 75, 50, TOP_LEFT, GRAY);
    else NewText(lobbySearch, 70, 75, 50, TOP_LEFT, BLACK);
    
    // Tag box
    NewRectangle(WIDTH / 2 + 300, 50, 250, 100, TOP_LEFT, (lobbySelected == 2 ? RED : BLACK));
    NewText("Tags (T)", WIDTH / 2 + 425, 75, 50, TOP_CENTER, BLACK);

    // tags
    NewRectangle(WIDTH / 2 + 300, 200, 320, HEIGHT - 300, TOP_LEFT, BLACK);
    for (int i = 0; i < 6; i++) {
        NewRectangle(WIDTH / 2 + 550, 220 + 90 * i, 50, 50, TOP_LEFT, (lobbyTagsSelected[i] ? RED : BLACK));
        NewText(tagNames[i], WIDTH / 2 + 320, 225 + 90 * i, 35, TOP_LEFT, BLACK);
        if (lobbySelected == 2) {
            NewText(tagNumbers[i], WIDTH / 2 + 575, 245 + 90 * i, 35, MIDDLE_CENTER, BLACK);
        }
    }

    // top right 
    NewText(TextFormat("Hello, %s!", lobbySchoolNumber), WIDTH - 50, 75, 50, TOP_RIGHT, BLACK);

    // Search Result Display
    NewRectangle(50, 200, WIDTH / 2 + 200, HEIGHT - 300, TOP_LEFT, BLACK);
    NewText(TextFormat("<- %d / %d ->", lobbyPage, lostItems->len / 5 + (lostItems->len % 5 ? 1 : 0)), WIDTH / 4 + 125, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    
    int result_height = (HEIGHT - 290) / 5;
    for (int i = 0; i < 5; i++) {
        int ind = (lobbyPage - 1) * 5 + i;
        if (ind < lobbySearchResultLength) {
            NewRectangle(50, 195 + result_height * i + result_height / 2, WIDTH / 2 + 200, result_height - 10, MIDDLE_LEFT, BLACK);
            NewText(TextFormat("(%d) %s", i + 1, lostItems->list[ind]->title), 55, 195 + result_height * i + result_height / 2, 40, MIDDLE_LEFT, BLACK);
        }
    }

    // Post    
    NewRectangle(WIDTH - 50, 200, 250, 150, TOP_RIGHT, BLACK);
    NewText("Post (P)", WIDTH - 175, 275, 50, MIDDLE_CENTER, BLACK);

    // DM
    NewRectangle(WIDTH - 50, 390, 250, 150, TOP_RIGHT, BLACK);
    NewText("DM (D)", WIDTH - 175, 465, 50, MIDDLE_CENTER, BLACK);

    // Logout
    NewRectangle(WIDTH - 50, 580, 250, 150, TOP_RIGHT, BLACK);
    NewText("Logout (L)", WIDTH - 175, 655, 40, MIDDLE_CENTER, BLACK);
}

void LobbyReset() {
    free(lobbySearchResult);
    lobbySync = false;
    memset(lobbySchoolNumber, '\0', 50);
    memset(lobbySearch, '\0', MAX_SEARCH_LEN);
    lobbySelected = 0;
    lobbyPage = 1;
    memset(lobbyTagsSelected, false, 6);
    lobbySearchResultLength = 0;
    lobbyTargetUserSelected = false;
}

void LobbyLostItemListSync() {
    lobbySearchResult = lostItems->search_lost_item(lostItems, lobbySearch, lobbyTagsSelected, (lobbyTargetUserSelected ? lobbySchoolNumber : lobbyDummyUser));
    lobbySearchResultLength = lost_item_list_length(lobbySearchResult);
}