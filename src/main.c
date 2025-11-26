#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static int scene = 1;
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
    register_reset();  // scene=2
    lobby_reset();     // scene=3
    post_reset();      // scene=4
    view_reset();      // scene=5
    notif_reset();    // scene=8

    // todo: remove this when finish
    // strcpy(schoolNumber, "24101");

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
                extern bool login_valid;
                extern char login_username[50];
                extern char login_password[50];

                login_screen();

                // DrawText(TextFormat("loginSelected: %d", login_selected), 20, HEIGHT - 120, 30, BLACK);
                // DrawText(TextFormat("loginValid: %d", login_valid), 20, HEIGHT - 160, 30, BLACK);
                // DrawText(TextFormat("loginUsername: %s", login_username), 20, HEIGHT - 200, 30, BLACK);
                // DrawText(TextFormat("loginPassword: %s", login_password), 20, HEIGHT - 240, 30, BLACK);

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
                            login_valid = 0;
                            strcpy(school_number, login_username);
                            scene = 3;
                        }
                        login_reset();
                    } else if (IsKeyPressed(KEY_B)) {
                        login_reset();
                        login_valid = 0;
                        scene = 0;
                    }
                }
                break;

            case 2:  // register
                extern int register_selected;
                extern bool register_valid;
                extern char register_username[50];
                extern char register_password[50];
                extern char register_password2[50];

                register_screen();

                // DrawText(TextFormat("registerSelected: %d", registerSelected), 20, HEIGHT - 120, 30, BLACK);
                // DrawText(TextFormat("registerValid: %d", registerValid), 20, HEIGHT - 160, 30, BLACK);
                // DrawText(TextFormat("registerUsername: %s", registerUsername), 20, HEIGHT - 200, 30, BLACK);
                // DrawText(TextFormat("registerPassword: %s", registerPassword), 20, HEIGHT - 240, 30, BLACK);
                // DrawText(TextFormat("registerPassword2: %s", registerPassword2), 20, HEIGHT - 280, 30, BLACK);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        register_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (register_selected == 1) register_username[strlen(register_username) - 1] = '\0';
                        else if (register_selected == 2) register_password[strlen(register_password) - 1] = '\0';
                        else if (register_selected == 3) register_password2[strlen(register_password2) - 1] = '\0';
                    } else {
                        if (register_selected == 1) register_username[strlen(register_username)] = GetCharPressed();
                        else if (register_selected == 2) register_password[strlen(register_password)] = GetCharPressed();
                        else if (register_selected == 3) register_password2[strlen(register_password2)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_U)) {
                        typing = true;
                        register_selected = 1;
                    } else if (IsKeyPressed(KEY_P)) {
                        typing = true;
                        register_selected = 2;
                    } else if (IsKeyPressed(KEY_C)) {
                        typing = true;
                        register_selected = 3;
                    } else if (IsKeyPressed(KEY_R)) {
                        if (register_check()) {
                            register_valid = 0;
                            scene = 0;
                        }
                        register_reset();
                    } else if (IsKeyPressed(KEY_B)) {
                        register_reset();
                        register_valid = 0;
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

                // DrawText(TextFormat("schoolNumber: %s", lobby_school_number), 20, HEIGHT - 120, 30, BLACK);
                // DrawText(TextFormat("search: %s", lobby_search), 20, HEIGHT - 160, 30, BLACK);
                // DrawText(TextFormat("selected: %d", lobby_selected), 20, HEIGHT - 200, 30, BLACK);
                // DrawText(TextFormat("page: %d", lobby_page), 20, HEIGHT - 240, 30, BLACK);

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
                        DM_reset();
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_L)) {
                        scene = 0;
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_N)) {
                        scene = 9;
                        notif_reset();
                        lobby_reset();
                    }
                }
                break;

            case 4:  // post
                extern int post_selected;
                extern bool post_tags_selected[6];
                extern char post_title[MAX_TITLE_LEN];
                extern char post_content[MAX_CONTENT_LEN];
                extern char post_target_user[MAX_USERNAME_LEN];
                extern char post_school_number[MAX_USERNAME_LEN];

                post_screen();
                strcpy(post_school_number, school_number);

                if (typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        typing = false;
                        post_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (post_selected == 1 && strlen(post_title) > 0) post_title[strlen(post_title) - 1] = '\0';
                        else if (post_selected == 2 && strlen(post_content) > 0) post_content[strlen(post_content) - 1] = '\0';
                        else if (post_selected == 3 && strlen(post_target_user) > 0) post_target_user[strlen(post_target_user) - 1] = '\0';
                    } else {
                        // todo: word count limit
                        if (post_selected == 1 && strlen(post_title) < MAX_TITLE_LEN - 1)
                            post_title[strlen(post_title)] = GetCharPressed();
                        else if (post_selected == 2 && strlen(post_content) < MAX_CONTENT_LEN - 1)
                            post_content[strlen(post_content)] = GetCharPressed();
                        else if (post_selected == 3 && strlen(post_target_user) < MAX_USERNAME_LEN - 1)
                            post_target_user[strlen(post_target_user)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_T)) {
                        typing = true;
                        post_selected = 1;
                    } else if (IsKeyPressed(KEY_C)) {
                        typing = true;
                        post_selected = 2;
                    } else if (IsKeyPressed(KEY_U)) {
                        typing = true;
                        post_selected = 3;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        post_tags_selected[0] = !post_tags_selected[0];
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        post_tags_selected[1] = !post_tags_selected[1];
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        post_tags_selected[2] = !post_tags_selected[2];
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        post_tags_selected[3] = !post_tags_selected[3];
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        post_tags_selected[4] = !post_tags_selected[4];
                    } else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                        post_tags_selected[5] = !post_tags_selected[5];
                    } else if (IsKeyPressed(KEY_P)) {
                        if (post_valid()) {
                            lost_items->insert_lost_item(lost_items, post_title, post_content, post_target_user, post_school_number, post_tags_selected, false);
                            save_new_lost_item(lost_items->list[lost_items->len - 1]);
                            post_reset();
                            scene = 3;
                        }
                    } else if (IsKeyPressed(KEY_B)) {
                        post_reset();
                        scene = 3;
                    }
                }
                break;

            case 5:  // view
                extern LostItem* view_item;
                extern char view_school_number[MAX_USERNAME_LEN];
                extern bool view_sync;
                view_item = lost_items->list[view_item_id];


                strcpy(view_school_number, school_number);

                if (!view_sync) {
                    view_synchronize();
                    view_sync = true;
                }

                view_screen();

                if (IsKeyPressed(KEY_B)) {
                    scene = 3;
                    view_reset();
                } else if (IsKeyPressed(KEY_D) && strcmp(view_item->writer, school_number) != 0) {
                    // todo: directly to DM
                    strcpy(DMsend_receiver, view_item->writer);
                    scene = 8;
                } else if (IsKeyPressed(KEY_M) && strcmp(view_item->writer, school_number) == 0) {
                    // mark as deleted
                    lost_items->delete_lost_item(lost_items, view_item_id);
                    view_reset();
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

                if (!typing) {
                    if (IsKeyPressed(KEY_S)) {
                        DM_types = 1;
                        DMListSync();
                    }
                    else if (IsKeyPressed(KEY_I)) {
                        DM_types = 2;
                        DMListSync();
                    } else if (IsKeyPressed(KEY_B)) {
                        DM_reset();
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
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        if ((DM_page - 1) * 7 + 1 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 1];
                            scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        if ((DM_page - 1) * 7 + 2 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 2];
                            scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        if ((DM_page - 1) * 7 + 3 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 3];
                            scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        if ((DM_page - 1) * 7 + 4 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 4];
                            scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                        if ((DM_page - 1) * 7 + 5 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 5];
                            scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_SEVEN) || IsKeyPressed(KEY_KP_7)) {
                        if ((DM_page - 1) * 7 + 6 < dm_search_result_length) {
                            viewDM_id = dm_search_result[(DM_page - 1) * 7 + 6];
                            scene = 7;
                            DM_reset();
                        }
                    }
                }
                break;

            case 7:  // DM view
                extern DMMessage* viewDM;
                extern int from_notif;

                printf("DEBUG: viewDMId = %d, dmMessages->len = %d\n", viewDM_id, dm_messages->len);

                viewDM = dm_messages->list[viewDM_id];
                DMview_screen();

                if (!typing) {
                    if (IsKeyPressed(KEY_B)) {
                        DMview_reset();
                        if (from_notif) {
                            scene = 9;
                            from_notif = 0;
                        } else {
                            scene = 6;
                        }
                    }
                }
                break;

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
                        DMview_reset();
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

            case 9:  // notifications
                extern int notif_sellected;
                extern char notif_lines[MAX_LINES][MAX_CONTENT_LEN];
                extern int notif_count;
                extern int notif_total_page;
                extern int current_page;
                extern char id[LINES_PER_PAGE][3][20];
                extern int DM_types;
                extern int from_notif;

                strcpy(DM_school_number, school_number);

                if (notif_sellected == 1) {
                    DMListSync();
                    read_notif_file();
                    notif_sellected = 0;
                }

                DrawText(TextFormat("notif_count: %d", notif_count), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("notif_total_page: %d", notif_total_page), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("current_page: %d", current_page), 20, HEIGHT - 200, 30, BLACK);

                notif_screen();

                if (IsKeyPressed(KEY_B)) {
                    scene = 3;
                } else if (IsKeyPressed(KEY_C)) {
                    clear_notif();
                    read_notif_file();
                    notif_count = 0;
                    notif_total_page = 1;
                    current_page = 1;
                } else if (IsKeyPressed(KEY_RIGHT)) {
                    if (current_page < notif_total_page) current_page++;
                    read_notif_file();
                } else if (IsKeyPressed(KEY_LEFT)) {
                    if (current_page > 1) current_page--;
                    read_notif_file();
                } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                    DM_types = 2;
                    read_notif_file();
                    if ((current_page - 1) * 5 < notif_count) {
                        viewDM_id = dm_messages->search_dm_by_elements(dm_messages, id[0][0], id[0][1], atoi(id[0][2]));
                        notif_reset();
                        scene = 7;
                        from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                    DM_types = 2;
                    read_notif_file();
                    if ((current_page - 1) * 5 + 1 < notif_count) {
                        viewDM_id = dm_messages->search_dm_by_elements(dm_messages, id[1][0], id[1][1], atoi(id[1][2]));
                        notif_reset();
                        scene = 7;
                        from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                    DM_types = 2;
                    read_notif_file();
                    if ((current_page - 1) * 5 + 2 < notif_count) {
                        viewDM_id = dm_messages->search_dm_by_elements(dm_messages, id[2][0], id[2][1], atoi(id[2][2]));
                        notif_reset();
                        scene = 7;
                        from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                    DM_types = 2;
                    read_notif_file();
                    if ((current_page - 1) * 5 + 3 < notif_count) {
                        viewDM_id = dm_messages->search_dm_by_elements(dm_messages, id[3][0], id[3][1], atoi(id[3][2]));
                        notif_reset();
                        scene = 7;
                        from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                    DM_types = 2;
                    read_notif_file();
                    if ((current_page - 1) * 5 + 4 < notif_count) {
                        viewDM_id = dm_messages->search_dm_by_elements(dm_messages, id[4][0], id[4][1], atoi(id[4][2]));
                        notif_reset();
                        scene = 7;
                        from_notif = 1;
                    }
                }
                break;
            
           
            
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
