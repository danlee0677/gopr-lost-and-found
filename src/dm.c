#include "../include/constants.h"
#include "../include/raylib.h"
#include "../include/textbox.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/dm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int DMselected = -1;
int DMs = 0;
int myDMs = 0;
int DMpage = 1;
char myDMList[128][256];
int *dmSearchResult;
int dmSearchResultLength = 0;
int DMtypes = 1;
bool DMsync = false;

char DMschoolNumber[50];

extern DMList *dmMessages;

void DMscreen() {
    NewRectangle(50, 55, WIDTH - 100, 100, TOP_LEFT, BLACK);
    NewText("DM", WIDTH / 2, 110, 70, MIDDLE_CENTER, BLACK);
<<<<<<< Updated upstream

    NewRectangle(50, 200, WIDTH / 2 + 200, HEIGHT - 300, TOP_LEFT, BLACK);
    NewText("<-         ->", WIDTH / 4 + 125, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);
    int denominator = dmSearchResultLength / 7 + (dmSearchResultLength % 7 ? 1 : 0);
    NewText(TextFormat("%d / %d", DMpage, (denominator ? denominator : 1)), WIDTH / 4 + 125, HEIGHT - 50, 50, MIDDLE_CENTER, BLACK);

    int result_height = (HEIGHT - 290) / 7;
    for (int i = 0; i < 7; i++) {
        int ind = (DMpage - 1) * 7 + i;
        if (ind < dmSearchResultLength) {
            NewRectangle(50, 195 + result_height * i + result_height / 2, WIDTH / 2 + 200, result_height - 10, MIDDLE_LEFT, BLACK);
            NewText(TextFormat("(%d) %s", i + 1, lostItems->list[dmSearchResult[ind]]->title), 70, 195 + result_height * i + result_height / 2, 40, MIDDLE_LEFT, BLACK);
        }
    }
=======
    
>>>>>>> Stashed changes
}

void DMreset() {
    DMselected = 0;
    DMs = 0;
    myDMs = 0;
    DMpage = 1;
    DMsync = false;
    DMtypes = 1;
    memset(myDMList, 0, sizeof(myDMList));
    memset(DMschoolNumber, 0, sizeof(DMschoolNumber));
}

void DMListSync() {
    DMpage = 1;
    dmSearchResult = dmMessages->search_message(dmMessages, DMschoolNumber, DMtypes);
    dmSearchResultLength = dm_list_length(dmSearchResult);
}