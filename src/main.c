#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//안태영ㅄ
#include "../include/constants.h"
#include "../include/dm.h"
#include "../include/draw.h"
#include "../include/lobby.h"
#include "../include/login.h"
#include "../include/menu.h"
#include "../include/models.h"
#include "../include/post.h"
#include "../include/raylib.h"
#include "../include/register.h"
#include "../include/view.h"
#include "../include/dmview.h"
#include "../include/notif.h"
#include "../include/dmsend.h"

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
extern LostItemList* lostItems;
extern DMList* dmMessages;
extern int viewItemId;
extern char DMsendReceiver[MAX_USERNAME_LEN];
extern int viewDMId;

int main() {
    InitWindow(WIDTH, HEIGHT, "LOST AND FOUND");
    SetTargetFPS(60);

    LoginReset();     // scene=1
    RegisterReset();  // scene=2
    LobbyReset();     // scene=3
    PostReset();      // scene=4
    ViewReset();      // scene=5

    // todo: remove this when finish
    strcpy(schoolNumber, "24101");

    load_lost_item_list(lostItems);
    load_dm_list(dmMessages);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("scene: %d", scene), 20, HEIGHT - 40, 30, BLACK);
        DrawText(TextFormat("typing: %d", typing), 20, HEIGHT - 80, 30, BLACK);

        switch (scene) {
            case 0:  // main menu
                MainMenuScreen();

                if (IsKeyPressed(KEY_L)) scene = 1;
                if (IsKeyPressed(KEY_R)) scene = 2;

                break;
            case 1:  // login
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
                        if (loginSelected == 1)
                            loginUsername[strlen(loginUsername) - 1] = '\0';
                        else if (loginSelected == 2)
                            loginPassword[strlen(loginPassword) - 1] = '\0';
                    } else {
                        if (loginSelected == 1)
                            loginUsername[strlen(loginUsername)] = GetCharPressed();
                        else if (loginSelected == 2)
                            loginPassword[strlen(loginPassword)] = GetCharPressed();
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
            case 2:  // register
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
                        if (registerSelected == 1)
                            registerUsername[strlen(registerUsername) - 1] = '\0';
                        else if (registerSelected == 2)
                            registerPassword[strlen(registerPassword) - 1] = '\0';
                        else if (registerSelected == 3)
                            registerPassword2[strlen(registerPassword2) - 1] = '\0';
                    } else {
                        if (registerSelected == 1)
                            registerUsername[strlen(registerUsername)] = GetCharPressed();
                        else if (registerSelected == 2)
                            registerPassword[strlen(registerPassword)] = GetCharPressed();
                        else if (registerSelected == 3)
                            registerPassword2[strlen(registerPassword2)] = GetCharPressed();
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
            case 3:  // lobby
                extern char lobbySchoolNumber[50];
                extern char lobbySearch[MAX_SEARCH_LEN];
                extern int lobbySelected;
                extern int lobbyPage;
                extern bool lobbyTagsSelected[6];
                extern int* lobbySearchResult;
                extern char lobbyDummyUser[1];
                extern bool lobbyTargetUserSelected;
                extern int lobbySearchResultLength;
                extern bool lobbySync;
                extern bool lobbyFilters[2];

                strcpy(lobbySchoolNumber, schoolNumber);

                if (!lobbySync) {
                    lobbySync = true;
                    LobbyLostItemListSync();
                    printf("%d\n", lobbySearchResultLength);
                }

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
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1))
                        lobbyTagsSelected[0] = !lobbyTagsSelected[0];
                    else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2))
                        lobbyTagsSelected[1] = !lobbyTagsSelected[1];
                    else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3))
                        lobbyTagsSelected[2] = !lobbyTagsSelected[2];
                    else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4))
                        lobbyTagsSelected[3] = !lobbyTagsSelected[3];
                    else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5))
                        lobbyTagsSelected[4] = !lobbyTagsSelected[4];
                    else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6))
                        lobbyTagsSelected[5] = !lobbyTagsSelected[5];
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
                    }
                    // navigate upon selecting
                    else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        if ((lobbyPage - 1) * 5 < lobbySearchResultLength) {
                            viewItemId = lobbySearchResult[(lobbyPage - 1) * 5];
                            scene = 5;
                            LobbyReset();
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        if ((lobbyPage - 1) * 5 + 1 < lobbySearchResultLength) {
                            viewItemId = lobbySearchResult[(lobbyPage - 1) * 5 + 1];
                            scene = 5;
                            LobbyReset();
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        if ((lobbyPage - 1) * 5 + 2 < lobbySearchResultLength) {
                            viewItemId = lobbySearchResult[(lobbyPage - 1) * 5 + 2];
                            scene = 5;
                            LobbyReset();
                        }
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        if ((lobbyPage - 1) * 5 + 3 < lobbySearchResultLength) {
                            viewItemId = lobbySearchResult[(lobbyPage - 1) * 5 + 3];
                            scene = 5;
                            LobbyReset();
                        }
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        if ((lobbyPage - 1) * 5 + 4 < lobbySearchResultLength) {
                            viewItemId = lobbySearchResult[(lobbyPage - 1) * 5 + 4];
                            scene = 5;
                            LobbyReset();
                        }
                    } else if (IsKeyPressed(KEY_A)) {
                        lobbyFilters[0] = !lobbyFilters[0];
                        LobbyLostItemListSync();
                    } else if (IsKeyPressed(KEY_W)) {
                        lobbyFilters[1] = !lobbyFilters[1];
                        LobbyLostItemListSync();
                    } else if (IsKeyPressed(KEY_C)) {
                        LobbyReset();
                    } else if (IsKeyPressed(KEY_P)) {
                        scene = 4;
                        LobbyReset();
                    } else if (IsKeyPressed(KEY_D)) {
                        scene = 6;
                        LobbyReset();
                    } else if (IsKeyPressed(KEY_L)) {
                        scene = 0;
                        LobbyReset();
                    } else if(IsKeyPressed(KEY_Q)) {
                        scene = 9;
                        LobbyReset();
                    }
                }

                break;
            case 4:  // post
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
                        if (postSelected == 1 && strlen(postTitle) > 0)
                            postTitle[strlen(postTitle) - 1] = '\0';
                        else if (postSelected == 2 && strlen(postContent) > 0)
                            postContent[strlen(postContent) - 1] = '\0';
                        else if (postSelected == 3 && strlen(postTargetUser) > 0)
                            postTargetUser[strlen(postTargetUser) - 1] = '\0';
                    } else {
                        // todo: word count limit
                        if (postSelected == 1 && strlen(postTitle) < MAX_TITLE_LEN - 1)
                            postTitle[strlen(postTitle)] = GetCharPressed();
                        else if (postSelected == 2 && strlen(postContent) < MAX_CONTENT_LEN - 1)
                            postContent[strlen(postContent)] = GetCharPressed();
                        else if (postSelected == 3 && strlen(postTargetUser) < MAX_USERNAME_LEN - 1)
                            postTargetUser[strlen(postTargetUser)] = GetCharPressed();
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
            case 5:  // view
                extern LostItem *viewItem;
                extern char viewSchoolNumber[MAX_USERNAME_LEN];
                extern bool viewSync;
                viewItem = lostItems->list[viewItemId];


                strcpy(viewSchoolNumber, schoolNumber);

                if (!viewSync) {
                    ViewSynchronize();
                    viewSync = true;
                }

                ViewScreen();

                if (IsKeyPressed(KEY_B)) {
                    // go back
                    scene = 3;
                    ViewReset();
                } else if (IsKeyPressed(KEY_D) && strcmp(viewItem->writer, schoolNumber) != 0) {
                    // todo: directly to DM
                    strcpy(DMsendReceiver, viewItem->writer);
                    scene = 8;
                } else if (IsKeyPressed(KEY_M) && strcmp(viewItem->writer, schoolNumber) == 0) {
                    // mark as deleted
                    lostItems->delete_lost_item(lostItems, viewItemId);
                    ViewReset();
                    scene = 3;
                }
                break;
            case 6:  // DM
                extern int DMselected;
                extern bool DMsync;
                extern char DMschoolNumber[50];
                extern int DMtypes;
                extern int dmSearchResultLength;
                extern int* dmSearchResult;
                extern int DMpage;

                strcpy(DMschoolNumber, schoolNumber);

                if (!DMsync) {
                    DMListSync();
                    DMsync = true;
                }

                DMscreen();

                if (typing) {
                } else {                    
                    if (IsKeyPressed(KEY_S)) {
                        DMtypes = 1;
                        DMListSync();
                    }
                    else if (IsKeyPressed(KEY_I)) {
                        DMtypes = 2;
                        DMListSync();
                    }
                    else if (IsKeyPressed(KEY_B)) {
                        DMreset();
                        scene = 3;
                    }

                    else if (IsKeyPressed(KEY_RIGHT)) {
                        if (DMpage < (dmSearchResultLength / 7) + (dmSearchResultLength % 7 ? 1 : 0)) DMpage++;
                    } else if (IsKeyPressed(KEY_LEFT)) {
                        if (DMpage > 1) DMpage--;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        if ((DMpage - 1) * 5 < dmSearchResultLength) {
                            viewDMId = dmSearchResult[(DMpage - 1) * 7];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        if ((DMpage - 1) * 7 + 1 < dmSearchResultLength) {
                            viewDMId = dmSearchResult[(DMpage - 1) * 7 + 1];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        if ((DMpage - 1) * 7 + 2 < dmSearchResultLength) {
                            viewDMId = dmSearchResult[(DMpage - 1) * 7 + 2];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        if ((DMpage - 1) * 7 + 3 < dmSearchResultLength) {
                            viewDMId = dmSearchResult[(DMpage - 1) * 7 + 3];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        if ((DMpage - 1) * 7 + 4 < dmSearchResultLength) {
                            viewDMId = dmSearchResult[(DMpage - 1) * 7 + 4];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                        if ((DMpage - 1) * 7 + 5 < dmSearchResultLength) {
                            viewDMId = dmSearchResult[(DMpage - 1) * 7 + 5];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_SEVEN) || IsKeyPressed(KEY_KP_7)) {
                        if ((DMpage - 1) * 7 + 6 < dmSearchResultLength) {
                            viewDMId = dmSearchResult[(DMpage - 1) * 7 + 6];
                            scene = 7;
                            DMreset();
                        }
                    }
                }
                break;

            case 7: //DM view
                extern DMMessage* viewDM;
                viewDM = dmMessages->list[viewDMId];

                DMviewScreen();

                if (typing) {
                } else {
                    if (IsKeyPressed(KEY_B)) {
                        DMviewReset();
                        scene = 6;
                    }
                }
            case 9:  // notifications
                extern char notif_lines[5][256];
                extern int notif_count;
                extern int notif_total_page;
                extern int current_page;
                int dmitemid;

                DrawText(TextFormat("notif_count: %d",  notif_count), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("notif_total_page: %d", notif_total_page), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("current_page: %d", current_page), 20, HEIGHT - 200, 30, BLACK);

                NotifScreen();
                if (IsKeyPressed(KEY_B)) {
                    scene = 3;
                } else if (IsKeyPressed(KEY_RIGHT)) {
                    if (current_page < notif_total_page? 1 : 0) current_page++;
                } else if (IsKeyPressed(KEY_LEFT)) {
                    if (current_page > 1) current_page--;
                } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                    if ((current_page - 1) * 5 < notif_count) {

                    }
                }  else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                    if ((current_page - 1) * 5 < notif_count) {

                    }
                }  else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                    if ((current_page - 1) * 5 < notif_count) {
                        
                    }
                }  else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                    if ((current_page - 1) * 5 < notif_count) {
                        
                    }
                }  else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                    if ((current_page - 1) * 5 < notif_count) {
                        
                    }
                } 

                break;
            
            // case 8: //DM send
            //     extern int DMsendSelected;
            //     extern char DMsendTitle[MAX_TITLE_LEN];
            //     extern char DMsendContent[MAX_CONTENT_LEN];
            //     extern char DMsendReceiver[MAX_USERNAME_LEN];
            //     extern char DMsendSchoolNumber[MAX_USERNAME_LEN];

            //     strcpy(DMsendSchoolNumber, schoolNumber);
                
            //     DMsendScreen();

            //     if (typing) {
            //         if (IsKeyPressed(KEY_ENTER)) {
            //             typing = false;
            //             DMsendSelected = 0;
            //         } else if (IsKeyPressed(KEY_BACKSPACE)) {
            //             if (DMsendSelected == 1 && strlen(DMsendTitle) > 0)
            //                 DMsendTitle[strlen(DMsendTitle) - 1] = '\0';
            //             else if (DMsendSelected == 2 && strlen(DMsendContent) > 0)
            //                 DMsendContent[strlen(DMsendContent) - 1] = '\0';
            //         } else {
            //             // todo: word count limit
            //             if (DMsendSelected == 1 && strlen(DMsendTitle) < MAX_TITLE_LEN - 1)
            //                 DMsendTitle[strlen(DMsendTitle)] = GetCharPressed();
            //             else if (DMsendSelected == 2 && strlen(DMsendContent) < MAX_CONTENT_LEN - 1)
            //                 DMsendContent[strlen(DMsendContent)] = GetCharPressed();
            //         }
            //     } else {
            //         if (IsKeyPressed(KEY_B)) {
            //             DMviewReset();
            //             scene = 5;
            //         } else if (IsKeyPressed(KEY_T)) {
            //             typing = true;
            //             DMsendSelected = 1;
            //         } else if (IsKeyPressed(KEY_C)) {
            //             typing = true;
            //             DMsendSelected = 2;
            //         } else if (IsKeyPressed(KEY_P)) {
            //             if (DMsendValid()) {
            //                 dmMessages->insert_message(dmMessages, DMsendTitle, DMsendContent, DMsendSchoolNumber, DMsendReceiver);
            //                 save_new_dm(dmMessages->list[dmMessages->len - 1]);
            //                 DMsendReset();
            //                 scene = 5;
            //             }
            //         }
            //     }
            //     break;
            
            case 8: //DM send
                extern int DMsendSelected;
                extern char DMsendTitle[MAX_TITLE_LEN];
                extern char DMsendContent[MAX_CONTENT_LEN];
                extern char DMsendReceiver[MAX_USERNAME_LEN];
                extern char DMsendSchoolNumber[MAX_USERNAME_LEN];

                strcpy(DMsendSchoolNumber, schoolNumber);
                
                DMsendScreen();

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        DMsendSelected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (DMsendSelected == 1 && strlen(DMsendTitle) > 0)
                            DMsendTitle[strlen(DMsendTitle) - 1] = '\0';
                        else if (DMsendSelected == 2 && strlen(DMsendContent) > 0)
                            DMsendContent[strlen(DMsendContent) - 1] = '\0';
                    } else {
                        // todo: word count limit
                        if (DMsendSelected == 1 && strlen(DMsendTitle) < MAX_TITLE_LEN - 1)
                            DMsendTitle[strlen(DMsendTitle)] = GetCharPressed();
                        else if (DMsendSelected == 2 && strlen(DMsendContent) < MAX_CONTENT_LEN - 1)
                            DMsendContent[strlen(DMsendContent)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_B)) {
                        DMviewReset();
                        scene = 5;
                    } else if (IsKeyPressed(KEY_T)) {
                        typing = true;
                        DMsendSelected = 1;
                    } else if (IsKeyPressed(KEY_C)) {
                        typing = true;
                        DMsendSelected = 2;
                    } else if (IsKeyPressed(KEY_P)) {
                        if (DMsendValid()) {
                            dmMessages->insert_message(dmMessages, DMsendTitle, DMsendContent, DMsendSchoolNumber, DMsendReceiver);
                            save_new_dm(dmMessages->list[dmMessages->len - 1]);
                            DMsendReset();
                            scene = 5;
                        }
                    }
                }
                break;
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}