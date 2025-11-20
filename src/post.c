#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/textbox.h"
#include "../include/post.h"


char postTitle[MAX_TITLE_LEN];
char postContent[MAX_CONTENT_LEN];
char postTargetUser[MAX_USERNAME_LEN];
char postSchoolNumber[MAX_USERNAME_LEN];
const char *postErrorMsg[] = {
    "",
    "Invalid Title",
    "Invalid Content",
    "Invalid Target User",
};
int postSelected = 0;
int postError = 0;
bool postTagsSelected[6] = {false, false, false, false, false, false};


void PostScreen() {
    // error msg
    if (postError) NewText(postErrorMsg[postError], WIDTH - 50, 30, 40, TOP_RIGHT, RED);
    
    // title
    NewRectangle(50, 85, WIDTH - 100, 55, TOP_LEFT, (postSelected == 1 ? RED : BLACK));
    NewText(TextFormat("%s", postTitle), 57, 112.5, 40, MIDDLE_LEFT, BLACK);
    NewText("Title (T)", 50, 30, 40, TOP_LEFT, BLACK);
    
    // content
    Rectangle TextBoxArea = {.x = 55, .y = 220, .width = WIDTH - 105, .height = HEIGHT - 360 - 220};
    NewText("Content (C)", 50, 165, 40, TOP_LEFT, BLACK);
    NewRectangle(50, 220, WIDTH - 100, HEIGHT - 360 - 220, TOP_LEFT, (postSelected == 2 ? RED : BLACK));
    DrawTextBoxed(GetFontDefault(), postContent, TextBoxArea, 40, 1, true, BLACK);

    // target_user
    // NewRectangle(50, 500, WIDTH - 100)
    NewText("Target User (U)", 50, HEIGHT - 335, 40, TOP_LEFT, BLACK);
    NewRectangle(50, HEIGHT - 280, WIDTH - 100, 50, TOP_LEFT, (postSelected == 3 ? RED : BLACK));
    NewText(postTargetUser, 55, HEIGHT - 255, 40, MIDDLE_LEFT, BLACK);

    // tags
    NewText("Tags", 50, HEIGHT - 215, 40, TOP_LEFT, BLACK);
    int tagWidth = (WIDTH - 250) / 6;
    const char *tagNames[6] = {"Electronics [1]", "Clothings [2]", "Bags [3]", "Stationery [4]", "Personal [5]", "Special [6]"};
    for (int i = 0; i < 6; i++) {
        NewRectangle(50 + (30 + tagWidth) * i, HEIGHT - 160, tagWidth, 50, TOP_LEFT, (postTagsSelected[i] ? RED : BLACK));
        NewText(tagNames[i], 50 + (tagWidth + 30) * i + tagWidth / 2, HEIGHT - 135, 30, MIDDLE_CENTER, BLACK);
    }
    

    // post / go back button
    // post
    NewRectangle(50, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    NewText("Back (B)", (WIDTH / 2 + 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
    // back
    NewRectangle(WIDTH / 2 + 25, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    NewText("Post (P)", (3 * WIDTH / 2 - 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
}

void PostReset() {
    memset(postTitle, '\0', MAX_TITLE_LEN);
    memset(postContent, '\0', MAX_CONTENT_LEN);
    memset(postTargetUser, '\0', MAX_USERNAME_LEN);
    memset(postTagsSelected, false, 6);
    postError = 0;
    postSelected = 0;
}

bool PostValid() {
    if (strlen(postTitle) == 0) {
        postError = 1;
        return false;
    } else if (strlen(postContent) == 0) {
        postError = 2;
        return false;
    } else if (strlen(postTargetUser) != 0 && strlen(postTargetUser) != 5) {
        postError = 3;
        return false;
    }
    postError = true;
    return true;
}