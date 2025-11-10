#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/login.h"

int loginSelected = 0;
int loginValid = 0;
char loginUsername[100];
char loginPassword[100];

void LoginScreen() {
    //draw the login screen text in the middle of the screen
    NewText("LOGIN", WIDTH / 2, 150, 100, MIDDLE_CENTER, BLACK);
    NewText("School Number (U)", WIDTH / 2 - 500, HEIGHT / 2 - 250, 70, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 - 500, HEIGHT / 2 - 150, 1000, 80, TOP_LEFT, (loginSelected == 1) ? RED : BLACK);
    NewText(loginUsername, WIDTH / 2 - 480, HEIGHT / 2 - 140, 60, TOP_LEFT, BLACK);
    NewText("Password (P)", WIDTH / 2 - 500, HEIGHT / 2 - 30, 70, TOP_LEFT, BLACK);
    NewRectangle(WIDTH / 2 - 500, HEIGHT / 2 + 70, 1000, 80, TOP_LEFT, (loginSelected == 2) ? RED : BLACK);
    NewText(loginPassword, WIDTH / 2 - 480, HEIGHT / 2 + 80, 60, TOP_LEFT, BLACK);
    NewText("Login (L)", WIDTH / 2, HEIGHT / 2 + 350, 70, MIDDLE_CENTER, BLACK);
    NewRectangle(WIDTH / 2, HEIGHT / 2 + 350, 400, 100, MIDDLE_CENTER, BLACK);
    NewText("Back (B)", WIDTH - 50, 50, 50, TOP_RIGHT, BLACK);
    NewRectangle(WIDTH - 30, 35, 250, 80, TOP_RIGHT, BLACK);
    switch(loginValid) {
        case 1:
        case 2:
            NewText("Invalid School Number", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
        case 3:
            NewText("Password must be 8-20 characters", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
        case 4:
            NewText("Invalid Password", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
        case 5:
            NewText("Login Failed", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
    }
}

static bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

static bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool LoginCheck() {
    //username must be 5 numbers, not other letters
    if (strlen(loginUsername) != 5) {
        loginValid = 1;
        return false;
    }

    int flag = 0;
    for (int i = 0; i < strlen(loginUsername); i++) {
        if (!isNumber(loginUsername[i])) flag = 1;
    }
    if (flag) {
        loginValid = 2;
        return false;
    }

    //password must be at least 8 characters
    if (strlen(loginPassword) < 8 || strlen(loginPassword) > 20) {
        loginValid = 3;
        return false;
    }
    //password must be consisted of numbers and letters
    flag = 0;
    for (int i = 0; i < strlen(loginPassword); i++) {
        if (!isNumber(loginPassword[i]) && !isLetter(loginPassword[i])) flag = 1;
    }
    if (flag) {
        loginValid = 4;
        return false;
    }

    //if the username and password are in the logindata.txt, return true
    FILE *fptr = fopen("./data/logindata.txt", "r");
    if (fptr == NULL) return false;
    char username[100], password[100];
    while (fscanf(fptr, "%s %s", username, password) != EOF) {
        if (strcmp(username, loginUsername) == 0 && strcmp(password, loginPassword) == 0) {
            fclose(fptr);
            return true;
        }
    }
    fclose(fptr);
    loginValid = 5;
    return false;
}

void LoginReset() {
    loginSelected = 0;
    memset(loginUsername, 0, sizeof(loginUsername));
    memset(loginPassword, 0, sizeof(loginPassword));
}