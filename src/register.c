#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/register.h"

int register_selected = 0;
int register_valid = 0;
char register_username[100];
char register_password[100];
char register_password2[100];

void register_screen() {
    //draw the login screen text in the middle of the screen
    draw_text("REGISTER", WIDTH / 2, 150, 100, MIDDLE_CENTER, BLACK);
    draw_text("School Number (U)", WIDTH / 2 - 500, HEIGHT / 2 - 290, 70, TOP_LEFT, BLACK);
    draw_rectangle(WIDTH / 2 - 500, HEIGHT / 2 - 200, 1000, 80, TOP_LEFT, (register_selected == 1) ? RED : BLACK);
    draw_text(register_username, WIDTH / 2 - 480, HEIGHT / 2 - 190, 60, TOP_LEFT, BLACK);
    draw_text("Password (P)", WIDTH / 2 - 500, HEIGHT / 2 - 90, 70, TOP_LEFT, BLACK);
    draw_rectangle(WIDTH / 2 - 500, HEIGHT / 2, 1000, 80, TOP_LEFT, (register_selected == 2) ? RED : BLACK);
    draw_text(register_password, WIDTH / 2 - 480, HEIGHT / 2 + 10, 60, TOP_LEFT, BLACK);
    draw_text("Confirm Password (C)", WIDTH / 2 - 500, HEIGHT / 2 + 100, 70, TOP_LEFT, BLACK);
    draw_rectangle(WIDTH / 2 - 500, HEIGHT / 2 + 190, 1000, 80, TOP_LEFT, (register_selected == 3) ? RED : BLACK);
    draw_text(register_password2, WIDTH / 2 - 480, HEIGHT / 2 + 200, 60, TOP_LEFT, BLACK);
    draw_text("Register (R)", WIDTH / 2, HEIGHT / 2 + 420, 70, MIDDLE_CENTER, BLACK);
    draw_rectangle(WIDTH / 2, HEIGHT / 2 + 420, 500, 100, MIDDLE_CENTER, BLACK);
    draw_text("Back (B)", WIDTH - 50, 50, 50, TOP_RIGHT, BLACK);
    draw_rectangle(WIDTH - 30, 35, 250, 80, TOP_RIGHT, BLACK);
    switch(register_valid) {
        case 1:
        case 2:
            draw_text("Invalid School Number", WIDTH / 2, HEIGHT / 2 + 320, 50, MIDDLE_CENTER, RED);
            break;
        case 3:
            draw_text("Password must be the same", WIDTH / 2, HEIGHT / 2 + 320, 50, MIDDLE_CENTER, RED);
            break;
        case 4:
            draw_text("Password must be 8-20 characters", WIDTH / 2, HEIGHT / 2 + 320, 50, MIDDLE_CENTER, RED);
            break;
        case 5:
            draw_text("Invalid Password", WIDTH / 2, HEIGHT / 2 + 320, 50, MIDDLE_CENTER, RED);
            break;
        case 6:
            draw_text("Already Registered", WIDTH / 2, HEIGHT / 2 + 320, 50, MIDDLE_CENTER, RED);
            break;
    }
}

static bool is_number(char c) {
    return c >= '0' && c <= '9';
}

static bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool register_check() {
    //username must be 5 numbers, not other letters
    if (strlen(register_username) != 5) {
        register_valid = 1;
        return false;
    }

    int flag = 0;
    for (int i = 0; i < strlen(register_username); i++) {
        if (!is_number(register_username[i])) flag = 1;
    }
    if (flag) {
        register_valid = 2;
        return false;
    }

    //password and password2 must be the same
    if (strcmp(register_password, register_password2) != 0) {
        register_valid = 3;
        return false;
    }
    //password must be at least 8 characters
    if (strlen(register_password) < 8 || strlen(register_password) > 20) {
        register_valid = 4;
        return false;
    }
    //password must be consisted of numbers and letters
    flag = 0;
    for (int i = 0; i < strlen(register_password); i++) {
        if (!is_number(register_password[i]) && !is_letter(register_password[i])) flag = 1;
    }
    if (flag) {
        register_valid = 5;
        return false;
    }

    //if the username already exists in the logindata.txt, return false
    FILE *fptr = fopen("./data/logindata.txt", "r");
    if (fptr == NULL) {
        printf("Error opening logindata.txt 1");
        return false;
    }
    char username[100], password[100];
    while (fscanf(fptr, "%s %s", username, password) != EOF) {
        if (strcmp(username, register_username) == 0) {
            register_valid = 6;
            return false;
        }
    }
    fclose(fptr);

    //if the username and password are valid, add it to the logindata.txt and return true
    fptr = fopen("./data/logindata.txt", "a+");
    if (fptr == NULL) {
        printf("Error opening logindata.txt 2");
        return false;
    }
    fprintf(fptr, "%s %s\n", register_username, register_password);
    fclose(fptr);
    return true;
}

void register_reset() {
    register_selected = 0;
    memset(register_username, 0, sizeof(register_username));
    memset(register_password, 0, sizeof(register_password));
    memset(register_password2, 0, sizeof(register_password2));
}