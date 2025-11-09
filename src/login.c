#include "../include/raylib.h"
#include "../include/login.h"
#include "../include/constants.h"
#include "../include/draw.h"

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
    NewText("LOGIN", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("School Number (U)", WIDTH / 2, HEIGHT / 2 - 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Password (P)", WIDTH / 2, HEIGHT / 2 + 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 - 50, 500, 100, MIDDLE_CENTER, BLACK);
}

void RegisterScreen() {
    //draw the register screen text in the middle of the screen
    NewText("REGISTER", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("School Number (U)", WIDTH / 2, HEIGHT / 2 - 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Password (P)", WIDTH / 2, HEIGHT / 2 + 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 - 50, 500, 100, MIDDLE_CENTER, BLACK);
}

void LoginUsernameSelected() {
    //draw the login screen text in the middle of the screen
    NewText("LOGIN", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("Password (P)", WIDTH / 2, HEIGHT / 2 + 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 + 50, 500, 100, MIDDLE_CENTER, BLACK);
}

void LoginPasswordSelected() {
    //draw the login screen text in the middle of the screen
    NewText("LOGIN", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("School Number (U)", WIDTH / 2, HEIGHT / 2 - 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 - 50, 500, 100, MIDDLE_CENTER, BLACK);
}

void RegisterUsernameSelected() {
    //draw the register screen text in the middle of the screen
    NewText("REGISTER", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("Password (P)", WIDTH / 2, HEIGHT / 2 + 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 + 50, 500, 100, MIDDLE_CENTER, BLACK);
}

void RegisterPasswordSelected() {
    //draw the register screen text in the middle of the screen
    NewText("REGISTER", WIDTH / 2, 200, 100, MIDDLE_CENTER, BLACK);
    NewText("School Number (U)", WIDTH / 2, HEIGHT / 2 - 50, 70, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH / 2, HEIGHT / 2 + 150, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 - 50, 500, 100, MIDDLE_CENTER, BLACK);
}