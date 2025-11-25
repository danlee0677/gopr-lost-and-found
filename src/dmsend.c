#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/models.h"
#include "../include/textbox.h"
#include "../include/dmsend.h"

char DMsendTitle[MAX_TITLE_LEN];
char DMsendContent[MAX_CONTENT_LEN];
char DMsendReceiver[MAX_USERNAME_LEN];
char DMsendSchoolNumber[MAX_USERNAME_LEN];
const char *DMsendErrorMsg[] = {
    "",
    "Invalid Title",
    "Invalid Content",
    "Invalid Target User",
};
int DMsendSelected = 0;
int DMsendError = 0;

void DMsendScreen() {
    // error msg
    if (DMsendError) NewText(DMsendErrorMsg[DMsendError], WIDTH - 50, 30, 40, TOP_RIGHT, RED);
    
    // title
    NewRectangle(50, 85, WIDTH - 100, 55, TOP_LEFT, (DMsendSelected == 1 ? RED : BLACK));
    NewText(TextFormat("%s", DMsendTitle), 57, 112.5, 40, MIDDLE_LEFT, BLACK);
    NewText("Title (T)", 50, 30, 40, TOP_LEFT, BLACK);
    
    // content
    Rectangle TextBoxArea = {.x = 55, .y = 220, .width = WIDTH - 105, .height = HEIGHT - 360 - 220};
    NewText("Content (C)", 50, 165, 40, TOP_LEFT, BLACK);
    NewRectangle(50, 220, WIDTH - 100, HEIGHT - 360 - 220, TOP_LEFT, (DMsendSelected == 2 ? RED : BLACK));
    DrawTextBoxed(GetFontDefault(), DMsendContent, TextBoxArea, 40, 1, true, BLACK);

    // target_user
    // NewRectangle(50, 500, WIDTH - 100)
    NewText("Receiver (U)", 50, HEIGHT - 335, 40, TOP_LEFT, BLACK);
    NewRectangle(50, HEIGHT - 280, WIDTH - 100, 50, TOP_LEFT, (DMsendSelected == 3 ? RED : BLACK));
    NewText(DMsendReceiver, 55, HEIGHT - 255, 40, MIDDLE_LEFT, BLACK);

    // post / go back button
    // post
    NewRectangle(50, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    NewText("Back (B)", (WIDTH / 2 + 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
    // back
    NewRectangle(WIDTH / 2 + 25, HEIGHT - 80, WIDTH / 2 - 75, 50, TOP_LEFT, BLACK);
    NewText("Post (P)", (3 * WIDTH / 2 - 25) / 2, (HEIGHT - 55), 40, MIDDLE_CENTER, BLACK);
}

void DMsendReset() {
    memset(DMsendTitle, '\0', MAX_TITLE_LEN);
    memset(DMsendContent, '\0', MAX_CONTENT_LEN);
    memset(DMsendReceiver, '\0', MAX_USERNAME_LEN);
    DMsendError = 0;
    DMsendSelected = 0;
}

bool DMsendValid() {
    if (strlen(DMsendTitle) == 0) {
        DMsendError = 1;
        return false;
    } else if (strlen(DMsendContent) == 0) {
        DMsendError = 2;
        return false;
    } else if (strlen(DMsendReceiver) != 0 && strlen(DMsendReceiver) != 5) {
        DMsendError = 3;
        return false;
    }
    DMsendError = true;
    return true;
}