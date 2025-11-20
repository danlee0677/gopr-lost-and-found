#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/raylib.h"
#include "../include/constants.h"
#include "../include/draw.h"
#include "../include/menu.h"
#include "../include/login.h"
#include "../include/register.h"
#include "../include/lobby.h"
#include "../include/models.h"
#include "../include/post.h"

/*
scene num - description
0 - main menu
1 - login
2 - register
3 - lobby
4 - post(post create)
5 - view(post viewer)
6 - DM
*/

static int scene = 3;
bool typing = false;
char schoolNumber[50];
extern LostItemList *lostItems;

int main() {
    InitWindow(WIDTH, HEIGHT, "LOST AND FOUND");
    SetTargetFPS(60);

    LoginReset(); // scene=1
    RegisterReset(); // scene=2
    LobbyReset(); // scene=3
    PostReset(); // scene=4


    // todo: remove this when finish
    strcpy(schoolNumber, "24077");

    load_lost_item_list(lostItems);

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
                extern int loginSelected;
                extern bool loginValid;
                extern char loginUsername[50];
                extern char loginPassword[50];

                LoginScreen();

                DrawText(TextFormat("loginSelected: %d", loginSelected), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("loginValid: %d", loginValid), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("loginUsername: %s", loginUsername), 20, HEIGHT - 200, 30, BLACK);
                DrawText(TextFormat("loginPassword: %s", loginPassword), 20, HEIGHT - 240, 30, BLACK);
                
                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        loginSelected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (loginSelected == 1) loginUsername[strlen(loginUsername) - 1] = '\0';
                        else if (loginSelected == 2) loginPassword[strlen(loginPassword) - 1] = '\0';
                    } else {
                        if (loginSelected == 1) loginUsername[strlen(loginUsername)] = GetCharPressed();
                        else if (loginSelected == 2) loginPassword[strlen(loginPassword)] = GetCharPressed();
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
                            loginValid = 0;
                            strcpy(schoolNumber, loginUsername);
                            scene = 3;
                        }
                        LoginReset();
                    } else if (IsKeyPressed(KEY_B)) {
                        LoginReset();
                        loginValid = 0;
                        scene = 0;
                    }
                }

                break;
            case 2: // register
                extern int registerSelected;
                extern bool registerValid;
                extern char registerUsername[50];
                extern char registerPassword[50];
                extern char registerPassword2[50];

                RegisterScreen();

                DrawText(TextFormat("registerSelected: %d", registerSelected), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("registerValid: %d", registerValid), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("registerUsername: %s", registerUsername), 20, HEIGHT - 200, 30, BLACK);
                DrawText(TextFormat("registerPassword: %s", registerPassword), 20, HEIGHT - 240, 30, BLACK);
                DrawText(TextFormat("registerPassword2: %s", registerPassword2), 20, HEIGHT - 280, 30, BLACK);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        registerSelected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (registerSelected == 1) registerUsername[strlen(registerUsername) - 1] = '\0';
                        else if (registerSelected == 2) registerPassword[strlen(registerPassword) - 1] = '\0';
                        else if (registerSelected == 3) registerPassword2[strlen(registerPassword2) - 1] = '\0';
                    } else {
                        if (registerSelected == 1) registerUsername[strlen(registerUsername)] = GetCharPressed();
                        else if (registerSelected == 2) registerPassword[strlen(registerPassword)] = GetCharPressed();
                        else if (registerSelected == 3) registerPassword2[strlen(registerPassword2)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_U)) {
                        typing = true;
                        registerSelected = 1;
                    } else if (IsKeyPressed(KEY_P)) {
                        typing = true;
                        registerSelected = 2;
                    } else if (IsKeyPressed(KEY_C)) {
                        typing = true;
                        registerSelected = 3;
                    } else if (IsKeyPressed(KEY_R)) {
                        if (RegisterCheck()) {
                            registerValid = 0;
                            scene = 0;
                        }
                        RegisterReset();
                    } else if (IsKeyPressed(KEY_B)) {
                        RegisterReset();
                        registerValid = 0;
                        scene = 0;
                    }
                }

                break;
            case 3: // lobby
                extern char lobbySchoolNumber[50];
                extern char lobbySearch[MAX_SEARCH_LEN];
                extern int lobbySelected;
                extern int lobbyPage;
                extern bool lobbyTagsSelected[6];
                extern int *lobbySearchResult;
                extern char lobbyDummyUser[1];
                extern bool lobbyTargetUserSelected;
                extern int lobbySearchResultLength;
                extern bool lobbySync;
                
                if (!lobbySync) {
                    lobbySync = true;
                    LobbyLostItemListSync();
                }

                strcpy(lobbySchoolNumber, schoolNumber);

                LobbyScreen();

                DrawText(TextFormat("schoolNumber: %s", lobbySchoolNumber), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("search: %s", lobbySearch), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("selected: %d", lobbySelected), 20, HEIGHT - 200, 30, BLACK);
                DrawText(TextFormat("page: %d", lobbyPage), 20, HEIGHT - 240, 30, BLACK);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        lobbySelected = 0;
                        free(lobbySearchResult);
                        LobbyLostItemListSync();
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (lobbySelected == 1 && strlen(lobbySearch) > 0) lobbySearch[strlen(lobbySearch) - 1] = '\0';
                    } else {
                        if (lobbySelected == 1 && strlen(lobbySearch) < MAX_SEARCH_LEN - 1) lobbySearch[strlen(lobbySearch)] = GetCharPressed();
                    }
                } else if (lobbySelected == 2) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        free(lobbySearchResult);
                        LobbyLostItemListSync();
                        lobbySelected = 0;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) lobbyTagsSelected[0] = !lobbyTagsSelected[0];
                    else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) lobbyTagsSelected[1] = !lobbyTagsSelected[1];
                    else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) lobbyTagsSelected[2] = !lobbyTagsSelected[2];
                    else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) lobbyTagsSelected[3] = !lobbyTagsSelected[3];
                    else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) lobbyTagsSelected[4] = !lobbyTagsSelected[4];
                    else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) lobbyTagsSelected[5] = !lobbyTagsSelected[5];
                } else {
                    if (IsKeyPressed(KEY_S)) {
                        typing = true;
                        lobbySelected = 1;
                    } else if (IsKeyPressed(KEY_T)) {
                        lobbySelected = 2;
                    } else if (IsKeyPressed(KEY_RIGHT)) {
                        if (lobbyPage < (lobbySearchResultLength / 5) + (lobbySearchResultLength % 5 ? 1 : 0)) lobbyPage++;
                    } else if (IsKeyPressed(KEY_LEFT)) {
                        if (lobbyPage > 1) lobbyPage--;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {

                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {

                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {

                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {

                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {

                    } else if (IsKeyPressed(KEY_P)) {
                        scene = 4;
                        LobbyReset();
                    } else if (IsKeyPressed(KEY_D)) {
                        scene = 6;
                        LobbyReset();
                    } else if (IsKeyPressed(KEY_L)) {
                        scene = 0;
                        LobbyReset();
                    }
                }

                break;
            case 4: // post
                extern int postSelected;
                extern bool postTagsSelected[6];
                extern char postTitle[MAX_TITLE_LEN];
                extern char postContent[MAX_CONTENT_LEN];
                extern char postTargetUser[MAX_USERNAME_LEN];
                extern char postSchoolNumber[MAX_USERNAME_LEN];

                PostScreen();
                strcpy(postSchoolNumber, schoolNumber);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        postSelected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (postSelected == 1 && strlen(postTitle) > 0) postTitle[strlen(postTitle) - 1] = '\0';
                        else if (postSelected == 2 && strlen(postContent) > 0) postContent[strlen(postContent) - 1] = '\0';
                        else if (postSelected == 3 && strlen(postTargetUser) > 0) postTargetUser[strlen(postTargetUser) - 1] = '\0';
                    } else {
                        // todo: word count limit
                        if (postSelected == 1 && strlen(postTitle) < MAX_TITLE_LEN - 1) postTitle[strlen(postTitle)] = GetCharPressed();
                        else if (postSelected == 2 && strlen(postContent) < MAX_CONTENT_LEN - 1) postContent[strlen(postContent)] = GetCharPressed();
                        else if (postSelected == 3 && strlen(postTargetUser) < MAX_USERNAME_LEN - 1) postTargetUser[strlen(postTargetUser)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_T)) {
                        typing = true;
                        postSelected = 1;
                    } else if (IsKeyPressed(KEY_C)) {
                        typing = true;
                        postSelected = 2;
                    } else if (IsKeyPressed(KEY_U)) {
                        typing = true;
                        postSelected = 3;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        postTagsSelected[0] = !postTagsSelected[0];
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        postTagsSelected[1] = !postTagsSelected[1];
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        postTagsSelected[2] = !postTagsSelected[2];
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        postTagsSelected[3] = !postTagsSelected[3];
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        postTagsSelected[4] = !postTagsSelected[4];
                    } else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                        postTagsSelected[5] = !postTagsSelected[5];
                    } else if (IsKeyPressed(KEY_P)) {
                        if (PostValid()) {
                            lostItems->insert_lost_item(lostItems, postTitle, postContent, postTargetUser, postSchoolNumber, postTagsSelected, false);
                            save_new_lost_item(lostItems->list[lostItems->len - 1]);
                            PostReset();
                            scene = 3;
                        }
                    } else if (IsKeyPressed(KEY_B)) {
                        PostReset();
                        scene = 3;
                    }
                }
                break;
            case 5: // view
                extern int view_id;
                break;
            case 6: // DM
                break;
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}