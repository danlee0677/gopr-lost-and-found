#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/post.h"


char postSchoolNumber[50];
char postTitle[MAX_TITLE_LEN];
char postContent[MAX_CONTENT_LEN];
char postTargetUser[MAX_USERNAME_LEN];
int postSelected = 0;
bool postTagsSelected[6] = {false, false, false, false, false, false};


void PostScreen() {
    NewRectangle(50, 100, WIDTH - 100, 75, TOP_LEFT, (postSelected == 1 ? RED : BLACK));
    NewText(TextFormat("Title (S)"), 50, 30, 60, TOP_LEFT, BLACK);

    DrawText
}