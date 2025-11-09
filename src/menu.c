#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/menu.h"

void MainMenuScreen() {
    //draw the main menu screen text in the middle of the screen
    NewText("LOST AND FOUND", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("Login (L)", WIDTH / 2, HEIGHT / 2 - 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Register (R)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 - 50, 500, 100, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 + 150, 500, 100, MIDDLE_CENTER, BLACK);
}