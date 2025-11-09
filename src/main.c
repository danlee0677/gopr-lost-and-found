#include <stdio.h>
#include <string.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/menu.h"
#include "../include/login.h"
#include "../include/register.h"

/*
scene num - description
0 - main menu
1 - login
2 - register
3 - lobby
4 - 
*/

static int scene = 0;
bool typing = false;

int main() {
    InitWindow(WIDTH, HEIGHT, "LOST AND FOUND");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("scene: %d", scene), 20, HEIGHT - 40, 30, BLACK);
        DrawText(TextFormat("typing: %d", typing), 20, HEIGHT - 80, 30, BLACK);

        switch (scene) {
            case 0: // main menu
                MainMenuScreen();

                if (IsKeyPressed(KEY_L)) scene = 1;
                if (IsKeyPressed(KEY_R)) scene = 2;

                break;
            case 1: // login
                LoginScreen();

                extern int loginSelected;
                extern char loginUsername[100];
                extern char loginPassword[100];

                DrawText(TextFormat("loginSelected: %d", loginSelected), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("loginUsername: %s", loginUsername), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("loginPassword: %s", loginPassword), 20, HEIGHT - 200, 30, BLACK);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        loginSelected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (loginSelected == 1) {
                            loginUsername[strlen(loginUsername) - 1] = '\0';
                        } else if (loginSelected == 2) {
                            loginPassword[strlen(loginPassword) - 1] = '\0';
                        }
                    } else {
                        if (loginSelected == 1) {
                            loginUsername[strlen(loginUsername)] = GetCharPressed();
                        } else if (loginSelected == 2) {
                            loginPassword[strlen(loginPassword)] = GetCharPressed();
                        }
                    }
                } else {
                    if (IsKeyPressed(KEY_U)) {
                        typing = true;
                        loginSelected = 1;
                    } else if (IsKeyPressed(KEY_P)) {
                        typing = true;
                        loginSelected = 2;
                    } else if (IsKeyPressed(KEY_L)) {
                        if (LoginCheck()) {
                            LoginReset();
                            typing = false;
                            scene = 3;
                        }
                    } else if (IsKeyPressed(KEY_B)) {
                        LoginReset();
                        typing = false;
                        scene = 0;
                    }
                }

                break;
            case 2: // register
                RegisterScreen();

                extern int registerSelected;
                extern char registerUsername[100];
                extern char registerPassword[100];

                DrawText(TextFormat("registerSelected: %d", registerSelected), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("registerUsername: %s", registerUsername), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("registerPassword: %s", registerPassword), 20, HEIGHT - 200, 30, BLACK);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        registerSelected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (registerSelected == 1) {
                            registerUsername[strlen(registerUsername) - 1] = '\0';
                        } else if (registerSelected == 2) {
                            registerPassword[strlen(registerPassword) - 1] = '\0';
                        }
                    } else {
                        if (registerSelected == 1) {
                            registerUsername[strlen(registerUsername)] = GetCharPressed();
                        } else if (registerSelected == 2) {
                            registerPassword[strlen(registerPassword)] = GetCharPressed();
                        }
                    }
                } else {
                    if (IsKeyPressed(KEY_U)) {
                        typing = true;
                        registerSelected = 1;
                    } else if (IsKeyPressed(KEY_P)) {
                        typing = true;
                        registerSelected = 2;
                    } else if (IsKeyPressed(KEY_L)) {
                        if (RegisterCheck()) {
                            RegisterReset();
                            typing = false;
                            scene = 3;
                        }
                    } else if (IsKeyPressed(KEY_B)) {
                        RegisterReset();
                        typing = false;
                        scene = 0;
                    }
                }
                
                break;
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}