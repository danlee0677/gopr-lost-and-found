#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/login.h"

static int scene = 0;

int main() {
    InitWindow(WIDTH, HEIGHT, "main");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (scene) {
            case 0:
                loginScreen();
                break;
            case 1:
                scene = 2;
                break;
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}