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
char school_number[50];
extern LostItemList* lost_items;
extern DMList* dm_messages;
extern int view_item_id;
extern char DMsend_receiver[MAX_USERNAME_LEN];
extern int viewDM_id;

int main() {
    InitWindow(WIDTH, HEIGHT, "LOST AND FOUND");
    SetTargetFPS(60);

    login_reset();     // scene=1
    RegisterReset();  // scene=2
    lobby_reset();     // scene=3
    PostReset();      // scene=4
    ViewReset();      // scene=5

    // todo: remove this when finish
    strcpy(school_number, "24101");

    load_lost_item_list(lost_items);
    load_dm_list(dm_messages);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("scene: %d", scene), 20, HEIGHT - 40, 30, BLACK);
        DrawText(TextFormat("typing: %d", typing), 20, HEIGHT - 80, 30, BLACK);

        switch (scene) {
            case 0:  // main menu
                menu_screen();

                if (IsKeyPressed(KEY_L)) scene = 1;
                if (IsKeyPressed(KEY_R)) scene = 2;

                break;
            case 1:  // login
                extern int login_selected;
                extern bool loginValid;
                extern char login_username[50];
                extern char login_password[50];

                login_screen();

                DrawText(TextFormat("loginSelected: %d", login_selected), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("loginValid: %d", loginValid), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("loginUsername: %s", login_username), 20, HEIGHT - 200, 30, BLACK);
                DrawText(TextFormat("loginPassword: %s", login_password), 20, HEIGHT - 240, 30, BLACK);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        login_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (login_selected == 1)
                            login_username[strlen(login_username) - 1] = '\0';
                        else if (login_selected == 2)
                            login_password[strlen(login_password) - 1] = '\0';
                    } else {
                        if (login_selected == 1)
                            login_username[strlen(login_username)] = GetCharPressed();
                        else if (login_selected == 2)
                            login_password[strlen(login_password)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_U)) {
                        typing = true;
                        login_selected = 1;
                    } else if (IsKeyPressed(KEY_P)) {
                        typing = true;
                        login_selected = 2;
                    } else if (IsKeyPressed(KEY_L)) {
                        if (login_check()) {
                            loginValid = 0;
                            strcpy(school_number, login_username);
                            scene = 3;
                        }
                        login_reset();
                    } else if (IsKeyPressed(KEY_B)) {
                        login_reset();
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
                extern char lobby_school_number[50];
                extern char lobby_search[MAX_SEARCH_LEN];
                extern int lobby_selected;
                extern int lobby_page;
                extern bool lobby_tags_selected[6];
                extern int* lobby_search_result;
                extern char lobby_dummy_user[1];
                extern bool lobby_target_user_selected;
                extern int lobby_search_result_length;
                extern bool lobby_sync;
                extern bool lobby_filters[2];

                strcpy(lobby_school_number, school_number);

                if (!lobby_sync) {
                    lobby_sync = true;
                    lobby_sync_lost_item_list();
                    printf("%d\n", lobby_search_result_length);
                }

                lobby_screen();

                DrawText(TextFormat("schoolNumber: %s", lobby_school_number), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("search: %s", lobby_search), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("selected: %d", lobby_selected), 20, HEIGHT - 200, 30, BLACK);
                DrawText(TextFormat("page: %d", lobby_page), 20, HEIGHT - 240, 30, BLACK);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        lobby_selected = 0;
                        free(lobby_search_result);
                        lobby_sync_lost_item_list();
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (lobby_selected == 1 && strlen(lobby_search) > 0) lobby_search[strlen(lobby_search) - 1] = '\0';
                    } else {
                        if (lobby_selected == 1 && strlen(lobby_search) < MAX_SEARCH_LEN - 1) lobby_search[strlen(lobby_search)] = GetCharPressed();
                    }
                } else if (lobby_selected == 2) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        free(lobby_search_result);
                        lobby_sync_lost_item_list();
                        lobby_selected = 0;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1))
                        lobby_tags_selected[0] = !lobby_tags_selected[0];
                    else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2))
                        lobby_tags_selected[1] = !lobby_tags_selected[1];
                    else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3))
                        lobby_tags_selected[2] = !lobby_tags_selected[2];
                    else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4))
                        lobby_tags_selected[3] = !lobby_tags_selected[3];
                    else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5))
                        lobby_tags_selected[4] = !lobby_tags_selected[4];
                    else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6))
                        lobby_tags_selected[5] = !lobby_tags_selected[5];
                } else {
                    if (IsKeyPressed(KEY_S)) {
                        typing = true;
                        lobby_selected = 1;
                    } else if (IsKeyPressed(KEY_T)) {
                        lobby_selected = 2;
                    } else if (IsKeyPressed(KEY_RIGHT)) {
                        if (lobby_page < (lobby_search_result_length / 5) + (lobby_search_result_length % 5 ? 1 : 0)) lobby_page++;
                    } else if (IsKeyPressed(KEY_LEFT)) {
                        if (lobby_page > 1) lobby_page--;
                    }
                    // navigate upon selecting
                    else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        if ((lobby_page - 1) * 5 < lobby_search_result_length) {
                            view_item_id = lobby_search_result[(lobby_page - 1) * 5];
                            scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        if ((lobby_page - 1) * 5 + 1 < lobby_search_result_length) {
                            view_item_id = lobby_search_result[(lobby_page - 1) * 5 + 1];
                            scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        if ((lobby_page - 1) * 5 + 2 < lobby_search_result_length) {
                            view_item_id = lobby_search_result[(lobby_page - 1) * 5 + 2];
                            scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        if ((lobby_page - 1) * 5 + 3 < lobby_search_result_length) {
                            view_item_id = lobby_search_result[(lobby_page - 1) * 5 + 3];
                            scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        if ((lobby_page - 1) * 5 + 4 < lobby_search_result_length) {
                            view_item_id = lobby_search_result[(lobby_page - 1) * 5 + 4];
                            scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_A)) {
                        lobby_filters[0] = !lobby_filters[0];
                        lobby_sync_lost_item_list();
                    } else if (IsKeyPressed(KEY_W)) {
                        lobby_filters[1] = !lobby_filters[1];
                        lobby_sync_lost_item_list();
                    } else if (IsKeyPressed(KEY_C)) {
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_P)) {
                        scene = 4;
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_D)) {
                        scene = 6;
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_L)) {
                        scene = 0;
                        lobby_reset();
                    } else if(IsKeyPressed(KEY_Q)) {
                        scene = 9;
                        lobby_reset();
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
                strcpy(postSchoolNumber, school_number);

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
                            lost_items->insert_lost_item(lost_items, postTitle, postContent, postTargetUser, postSchoolNumber, postTagsSelected, false);
                            save_new_lost_item(lost_items->list[lost_items->len - 1]);
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
                viewItem = lost_items->list[view_item_id];


                strcpy(viewSchoolNumber, school_number);

                if (!viewSync) {
                    ViewSynchronize();
                    viewSync = true;
                }

                ViewScreen();

                if (IsKeyPressed(KEY_B)) {
                    // go back
                    scene = 3;
                    ViewReset();
                } else if (IsKeyPressed(KEY_D) && strcmp(viewItem->writer, school_number) != 0) {
                    // todo: directly to DM
                    strcpy(DMsend_receiver, viewItem->writer);
                    scene = 8;
                } else if (IsKeyPressed(KEY_M) && strcmp(viewItem->writer, school_number) == 0) {
                    // mark as deleted
                    lost_items->delete_lost_item(lost_items, view_item_id);
                    ViewReset();
                    scene = 3;
                }
                break;
            case 6:  // DM
                extern int dm_selected;
                extern bool DM_sync;
                extern char DM_school_number[50];
                extern int DM_types;
                extern int dm_search_result_length;
                extern int* dm_search_result;
                extern int DM_page;

                strcpy(DM_school_number, school_number);

                if (!DM_sync) {
                    DMListSync();
                    DM_sync = true;
                }

                DMscreen();

                if (typing) {
                } else {                    
                    if (IsKeyPressed(KEY_S)) {
                        DM_types = 1;
                        DMListSync();
                    }
                    else if (IsKeyPressed(KEY_I)) {
                        DM_types = 2;
                        DMListSync();
                    }
                    else if (IsKeyPressed(KEY_B)) {
                        DMreset();
                        scene = 3;
                    }

                    else if (IsKeyPressed(KEY_RIGHT)) {
                        if (DM_page < (dm_search_result_length / 7) + (dm_search_result_length % 7 ? 1 : 0)) DM_page++;
                    } else if (IsKeyPressed(KEY_LEFT)) {
                        if (DM_page > 1) DM_page--;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        if ((DM_page - 1) * 5 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        if ((DM_page - 1) * 7 + 1 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 1];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        if ((DM_page - 1) * 7 + 2 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 2];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        if ((DM_page - 1) * 7 + 3 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 3];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        if ((DM_page - 1) * 7 + 4 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 4];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                        if ((DM_page - 1) * 7 + 5 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 5];
                            scene = 7;
                            DMreset();
                        }
                    } else if (IsKeyPressed(KEY_SEVEN) || IsKeyPressed(KEY_KP_7)) {
                        if ((DM_page - 1) * 7 + 6 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 6];
                            scene = 7;
                            DMreset();
                        }
                    }
                }
                break;

            case 7: //DM view
                extern DMMessage* viewDM;
                viewDM = dm_messages->list[viewDM_id];

                DMview_screen();

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
                extern int DMsend_selected;
                extern char DMsend_title[MAX_TITLE_LEN];
                extern char DMsend_content[MAX_CONTENT_LEN];
                extern char DMsend_receiver[MAX_USERNAME_LEN];
                extern char DMsend_school_number[MAX_USERNAME_LEN];

                strcpy(DMsend_school_number, school_number);
                
                DMsend_screen();

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        DMsend_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (DMsend_selected == 1 && strlen(DMsend_title) > 0)
                            DMsend_title[strlen(DMsend_title) - 1] = '\0';
                        else if (DMsend_selected == 2 && strlen(DMsend_content) > 0)
                            DMsend_content[strlen(DMsend_content) - 1] = '\0';
                    } else {
                        // todo: word count limit
                        if (DMsend_selected == 1 && strlen(DMsend_title) < MAX_TITLE_LEN - 1)
                            DMsend_title[strlen(DMsend_title)] = GetCharPressed();
                        else if (DMsend_selected == 2 && strlen(DMsend_content) < MAX_CONTENT_LEN - 1)
                            DMsend_content[strlen(DMsend_content)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_B)) {
                        DMviewReset();
                        scene = 5;
                    } else if (IsKeyPressed(KEY_T)) {
                        typing = true;
                        DMsend_selected = 1;
                    } else if (IsKeyPressed(KEY_C)) {
                        typing = true;
                        DMsend_selected = 2;
                    } else if (IsKeyPressed(KEY_P)) {
                        if (DMsend_valid()) {
                            dm_messages->insert_message(dm_messages, DMsend_title, DMsend_content, DMsend_school_number, DMsend_receiver);
                            save_new_dm(dm_messages->list[dm_messages->len - 1]);
                            DMsend_reset();
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