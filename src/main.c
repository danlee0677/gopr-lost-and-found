#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/login.h"

/*
scene num - description
0 - main menu
1 - login
2 - register
3 - lobby
4 - 
*/

static int scene = 0;

int main() {
    InitWindow(WIDTH, HEIGHT, "main");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("scene: %d", scene), 30, 30, 30, BLACK);

        switch (scene) {
            case 0: // main menu
                MainMenuScreen();

                if (IsKeyPressed(KEY_L)) scene = 1;
                if (IsKeyPressed(KEY_R)) scene = 2;

                break;
            case 1: // login
                LoginScreen();

                if (IsKeyPressed(KEY_U)) LoginUsernameSelected();
                if (IsKeyPressed(KEY_P)) LoginPasswordSelected();
                if (IsKeyPressed(KEY_B)) scene = 0;

                break;
            case 2: // register
                RegisterScreen();

                if (IsKeyPressed(KEY_U)) RegisterUsernameSelected();
                if (IsKeyPressed(KEY_P)) RegisterPasswordSelected();
                if (IsKeyPressed(KEY_B)) scene = 0;

                break;
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}