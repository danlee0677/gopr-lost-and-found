#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/register.h"

int registerSelected = 0;
char registerUsername[100];
char registerPassword[100];

void RegisterScreen() {
    //draw the login screen text in the middle of the screen
    NewText("REGISTER", WIDTH / 2, 150, 100, MIDDLE_CENTER, BLACK);
    NewText("School Number (U)", WIDTH / 2 - 500, HEIGHT / 2 - 250, 70, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 - 500, HEIGHT / 2 - 150, 1000, 80, TOP_LEFT, (registerSelected == 1) ? RED : BLACK);
    NewText(registerUsername, WIDTH / 2 - 480, HEIGHT / 2 - 140, 60, TOP_LEFT, BLACK);
    NewText("Password (P)", WIDTH / 2 - 500, HEIGHT / 2, 70, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 - 500, HEIGHT / 2 + 100, 1000, 80, TOP_LEFT, (registerSelected == 2) ? RED : BLACK);
    NewText(registerPassword, WIDTH / 2 - 480, HEIGHT / 2 + 110, 60, TOP_LEFT, BLACK);
    NewText("Login (L)", WIDTH / 2, HEIGHT / 2 + 350, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 + 350, 400, 100, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH - 50, 50, 50, TOP_RIGHT, BLACK);
    NewRectangle(WIDTH - 30, 35, 250, 80, TOP_RIGHT, BLACK);
}

bool RegisterCheck() {
    //username must be 5 numbers
    if (strlen(registerUsername) != 5) return false;
    return true;
}

void RegisterReset() {
    registerSelected = 0;
    registerUsername[0] = '\0';
    registerPassword[0] = '\0';
}