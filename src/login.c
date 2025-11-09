#include "../include/raylib.h"
#include "../include/login.h"
#include "../include/constants.h"
#include "../include/draw.h"

int loginSelected = 0;

char loginUsername[100];
char loginPassword[100];

void MainMenuScreen() {
    //draw the main menu screen text in the middle of the screen
    NewText("LOST AND FOUND", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("Login (L)", WIDTH / 2, HEIGHT / 2 - 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Register (R)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 - 50, 500, 100, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 + 150, 500, 100, MIDDLE_CENTER, BLACK);
}

void LoginScreen() {
    //draw the login screen text in the middle of the screen
    NewText("LOGIN", WIDTH / 2, 150, 100, MIDDLE_CENTER, BLACK);
    NewText("School Number (U)", WIDTH / 2 - 500, HEIGHT / 2 - 250, 70, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 - 500, HEIGHT / 2 - 150, 1000, 80, TOP_LEFT, (loginSelected == 1) ? RED : BLACK);
    NewText(loginUsername, WIDTH / 2 - 480, HEIGHT / 2 - 140, 60, TOP_LEFT, BLACK);
    NewText("Password (P)", WIDTH / 2 - 500, HEIGHT / 2, 70, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 - 500, HEIGHT / 2 + 100, 1000, 80, TOP_LEFT, (loginSelected == 2) ? RED : BLACK);
    NewText(loginPassword, WIDTH / 2 - 480, HEIGHT / 2 + 110, 60, TOP_LEFT, BLACK);
    NewText("Login (L)", WIDTH / 2, HEIGHT / 2 + 350, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 + 350, 400, 100, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH - 50, 50, 50, TOP_RIGHT, BLACK);
    NewRectangle(WIDTH - 30, 35, 250, 80, TOP_RIGHT, BLACK);
}

void LoginReset() {
    loginSelected = 0;
    loginUsername[0] = '\0';
    loginPassword[0] = '\0';
}