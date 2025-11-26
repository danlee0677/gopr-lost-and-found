#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/login.h"

int g_login_selected = 0;
int g_login_valid = 0;
char g_login_username[50];
char g_login_password[50];

void login_screen() {
    //draw the login screen text in the middle of the screen
    draw_text("LOGIN", WIDTH / 2, 150, 100, MIDDLE_CENTER, BLACK);
    draw_text("School Number (U)", WIDTH / 2 - 500, HEIGHT / 2 - 250, 70, TOP_LEFT, BLACK);
    draw_rectangle(WIDTH / 2 - 500, HEIGHT / 2 - 150, 1000, 80, TOP_LEFT, (g_login_selected == 1) ? RED : BLACK);
    draw_text(g_login_username, WIDTH / 2 - 480, HEIGHT / 2 - 140, 60, TOP_LEFT, BLACK);
    draw_text("Password (P)", WIDTH / 2 - 500, HEIGHT / 2 - 30, 70, TOP_LEFT, BLACK);
    draw_rectangle(WIDTH / 2 - 500, HEIGHT / 2 + 70, 1000, 80, TOP_LEFT, (g_login_selected == 2) ? RED : BLACK);
    draw_text(g_login_password, WIDTH / 2 - 480, HEIGHT / 2 + 80, 60, TOP_LEFT, BLACK);
    draw_text("Login (L)", WIDTH / 2, HEIGHT / 2 + 350, 70, MIDDLE_CENTER, BLACK);
    draw_rectangle(WIDTH / 2, HEIGHT / 2 + 350, 400, 100, MIDDLE_CENTER, BLACK);
    draw_text("Back (B)", WIDTH - 50, 50, 50, TOP_RIGHT, BLACK);
    draw_rectangle(WIDTH - 30, 35, 250, 80, TOP_RIGHT, BLACK);
    switch(g_login_valid) {
        case 1:
        case 2:
            draw_text("Invalid School Number", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
        case 3:
            draw_text("Password must be 8-20 characters", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
        case 4:
            draw_text("Invalid Password", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
        case 5:
            draw_text("Login Failed", WIDTH / 2, HEIGHT / 2 + 220, 50, MIDDLE_CENTER, RED);
            break;
    }
}

static bool is_number(char c) {
    return c >= '0' && c <= '9';
}

static bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool login_check() {
    //username must be 5 numbers, not other letters
    if (strlen(g_login_username) != 5) {
        g_login_valid = 1;
        return false;
    }

    int flag = 0;
    for (int i = 0; i < strlen(g_login_username); i++) {
        if (!is_number(g_login_username[i])) flag = 1;
    }
    if (flag) {
        g_login_valid = 2;
        return false;
    }

    //password must be at least 8 characters
    if (strlen(g_login_password) < 8 || strlen(g_login_password) > 20) {
        g_login_valid = 3;
        return false;
    }
    //password must be consisted of numbers and letters
    flag = 0;
    for (int i = 0; i < strlen(g_login_password); i++) {
        if (!is_number(g_login_password[i]) && !is_letter(g_login_password[i])) flag = 1;
    }
    if (flag) {
        g_login_valid = 4;
        return false;
    }

    //if the username and password are in the logindata.txt, return true
    FILE *fptr = fopen("./data/logindata.txt", "r");
    if (fptr == NULL) return false;
    char username[100], password[100];
    while (fscanf(fptr, "%s %s", username, password) != EOF) {
        if (strcmp(username, g_login_username) == 0 && strcmp(password, g_login_password) == 0) {
            fclose(fptr);
            return true;
        }
    }
    fclose(fptr);
    g_login_valid = 5;
    return false;
}

void login_reset() {
    g_login_selected = 0;
    memset(g_login_username, 0, sizeof(g_login_username));
    memset(g_login_password, 0, sizeof(g_login_password));
}