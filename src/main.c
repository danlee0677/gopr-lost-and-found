/*
main.c: 이벤트 처리 등
2025-11-26 이승준 변수 camel -> snake, 전역변수 앞에 g_, static 앞에 s_, 함수명 변경
2025-11-26 김현성 notif(공지) 추가(notif.c, scene = 9)
2025-11-26 안태영 DM 보내기 추가(dmsend.c, scene = 8)
2025-11-25 안태영 DM 뷰어 추가(dmview.c, scene = 7)
2025-11-25 안태영 DM 메인화면(dm.c scene = 6)
2025-11-21 이승준 분실물 보는 화면(view.c scene = 5)
2025-11-21 이승준 로비 인터페이스 미세조정, pagination 구현, 필터링 구현
2025-11-20 이승준 로비 로직 구현(태그/검색어 검색, lobby.c scene = 3)
2025-11-20 이승준 분실물 게시 화면(post.c, scene = 4) 구현완료
2025-11-19 정용주 lobby.c 검색 영역, 뒤로가기 영역 분할
2025-11-13 정용주 lobby.c 파일 생성
2025-11-10 정용주 register 화면 구현(register.c, scene = 2)
2025-11-09 정용주 맨 처음 화면 구현(menu.c, scene = 0)
2025-11-09 정용주 login 화면 구현(login.c, scene = 1)
2025-11-05 정용주 폴더 구조 구성 및 raylib 추가
*/
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
6 - DM list
7 - DM post
8 - DM view
9 - notifications
*/

static int s_scene = 0;
bool g_typing = false;
char g_school_number[50];
extern LostItemList* g_lost_items;
extern DMList* g_dm_messages;
extern int g_view_item_id;
extern char g_DMsend_receiver[MAX_USERNAME_LEN];
extern int g_viewDM_id;

int main() {
    InitWindow(WIDTH, HEIGHT, "LOST AND FOUND");
    SetTargetFPS(60);

    g_dm_messages = create_dm_list();
    g_lost_items = create_lost_item_list();

    login_reset();     // scene=1
    register_reset();  // scene=2
    lobby_reset();     // scene=3
    post_reset();      // scene=4
    view_reset();      // scene=5
    notif_reset();    // scene=9

    // todo: remove this when finish
    // strcpy(schoolNumber, "24101");

    load_lost_item_list(g_lost_items);
    load_dm_list(g_dm_messages);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("scene: %d", s_scene), 20, HEIGHT - 40, 30, BLACK);
        DrawText(TextFormat("typing: %d", g_typing), 20, HEIGHT - 80, 30, BLACK);

        switch (s_scene) {
            case 0:  // main menu
                menu_screen();

                if (IsKeyPressed(KEY_L)) s_scene = 1;
                if (IsKeyPressed(KEY_R)) s_scene = 2;
                break;

            case 1:  // login
                extern int g_login_selected;
                extern bool g_login_valid;
                extern char g_login_username[50];
                extern char g_login_password[50];

                login_screen();

                // DrawText(TextFormat("loginSelected: %d", login_selected), 20, HEIGHT - 120, 30, BLACK);
                // DrawText(TextFormat("loginValid: %d", login_valid), 20, HEIGHT - 160, 30, BLACK);
                // DrawText(TextFormat("loginUsername: %s", login_username), 20, HEIGHT - 200, 30, BLACK);
                // DrawText(TextFormat("loginPassword: %s", login_password), 20, HEIGHT - 240, 30, BLACK);

                if (g_typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        g_typing = false;
                        g_login_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (g_login_selected == 1)
                            g_login_username[strlen(g_login_username) - 1] = '\0';
                        else if (g_login_selected == 2)
                            g_login_password[strlen(g_login_password) - 1] = '\0';
                    } else {
                        if (g_login_selected == 1)
                            g_login_username[strlen(g_login_username)] = GetCharPressed();
                        else if (g_login_selected == 2)
                            g_login_password[strlen(g_login_password)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_U)) {
                        g_typing = true;
                        g_login_selected = 1;
                    } else if (IsKeyPressed(KEY_P)) {
                        g_typing = true;
                        g_login_selected = 2;
                    } else if (IsKeyPressed(KEY_L)) {
                        if (login_check()) {
                            g_login_valid = 0;
                            strcpy(g_school_number, g_login_username);
                            s_scene = 3;
                        }
                        login_reset();
                    } else if (IsKeyPressed(KEY_B)) {
                        login_reset();
                        g_login_valid = 0;
                        s_scene = 0;
                    }
                }
                break;

            case 2:  // register
                extern int g_register_selected;
                extern bool g_register_valid;
                extern char g_register_username[50];
                extern char g_register_password[50];
                extern char g_register_password2[50];

                register_screen();

                // DrawText(TextFormat("registerSelected: %d", registerSelected), 20, HEIGHT - 120, 30, BLACK);
                // DrawText(TextFormat("registerValid: %d", registerValid), 20, HEIGHT - 160, 30, BLACK);
                // DrawText(TextFormat("registerUsername: %s", registerUsername), 20, HEIGHT - 200, 30, BLACK);
                // DrawText(TextFormat("registerPassword: %s", registerPassword), 20, HEIGHT - 240, 30, BLACK);
                // DrawText(TextFormat("registerPassword2: %s", registerPassword2), 20, HEIGHT - 280, 30, BLACK);

                if (g_typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        g_typing = false;
                        g_register_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (g_register_selected == 1) g_register_username[strlen(g_register_username) - 1] = '\0';
                        else if (g_register_selected == 2) g_register_password[strlen(g_register_password) - 1] = '\0';
                        else if (g_register_selected == 3) g_register_password2[strlen(g_register_password2) - 1] = '\0';
                    } else {
                        if (g_register_selected == 1) g_register_username[strlen(g_register_username)] = GetCharPressed();
                        else if (g_register_selected == 2) g_register_password[strlen(g_register_password)] = GetCharPressed();
                        else if (g_register_selected == 3) g_register_password2[strlen(g_register_password2)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_U)) {
                        g_typing = true;
                        g_register_selected = 1;
                    } else if (IsKeyPressed(KEY_P)) {
                        g_typing = true;
                        g_register_selected = 2;
                    } else if (IsKeyPressed(KEY_C)) {
                        g_typing = true;
                        g_register_selected = 3;
                    } else if (IsKeyPressed(KEY_R)) {
                        if (register_check()) {
                            g_register_valid = 0;
                            s_scene = 0;
                        }
                        register_reset();
                    } else if (IsKeyPressed(KEY_B)) {
                        register_reset();
                        g_register_valid = 0;
                        s_scene = 0;
                    }
                }
                break;

            case 3:  // lobby
                extern char g_lobby_school_number[50];
                extern char g_lobby_search[MAX_SEARCH_LEN];
                extern int g_lobby_selected;
                extern int g_lobby_page;
                extern bool g_lobby_tags_selected[6];
                extern int* g_lobby_search_result;
                extern char g_lobby_dummy_user[1];
                extern bool g_lobby_target_user_selected;
                extern int g_lobby_search_result_length;
                extern bool g_lobby_sync;
                extern bool g_lobby_filters[2];

                strcpy(g_lobby_school_number, g_school_number);

                if (!g_lobby_sync) {
                    g_lobby_sync = true;
                    lobby_sync_lost_item_list();
                    printf("%d\n", g_lobby_search_result_length);
                }

                lobby_screen();

                // DrawText(TextFormat("schoolNumber: %s", lobby_school_number), 20, HEIGHT - 120, 30, BLACK);
                // DrawText(TextFormat("search: %s", lobby_search), 20, HEIGHT - 160, 30, BLACK);
                // DrawText(TextFormat("selected: %d", lobby_selected), 20, HEIGHT - 200, 30, BLACK);
                // DrawText(TextFormat("page: %d", lobby_page), 20, HEIGHT - 240, 30, BLACK);

                if (g_typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        g_typing = false;
                        g_lobby_selected = 0;
                        free(g_lobby_search_result);
                        lobby_sync_lost_item_list();
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (g_lobby_selected == 1 && strlen(g_lobby_search) > 0) g_lobby_search[strlen(g_lobby_search) - 1] = '\0';
                    } else {
                        if (g_lobby_selected == 1 && strlen(g_lobby_search) < MAX_SEARCH_LEN - 1) g_lobby_search[strlen(g_lobby_search)] = GetCharPressed();
                    }
                } else if (g_lobby_selected == 2) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        free(g_lobby_search_result);
                        lobby_sync_lost_item_list();
                        g_lobby_selected = 0;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1))
                        g_lobby_tags_selected[0] = !g_lobby_tags_selected[0];
                    else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2))
                        g_lobby_tags_selected[1] = !g_lobby_tags_selected[1];
                    else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3))
                        g_lobby_tags_selected[2] = !g_lobby_tags_selected[2];
                    else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4))
                        g_lobby_tags_selected[3] = !g_lobby_tags_selected[3];
                    else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5))
                        g_lobby_tags_selected[4] = !g_lobby_tags_selected[4];
                    else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6))
                        g_lobby_tags_selected[5] = !g_lobby_tags_selected[5];
                } else {
                    if (IsKeyPressed(KEY_S)) {
                        g_typing = true;
                        g_lobby_selected = 1;
                    } else if (IsKeyPressed(KEY_T)) {
                        g_lobby_selected = 2;
                    } else if (IsKeyPressed(KEY_RIGHT)) {
                        if (g_lobby_page < (g_lobby_search_result_length / 5) + (g_lobby_search_result_length % 5 ? 1 : 0)) g_lobby_page++;
                    } else if (IsKeyPressed(KEY_LEFT)) {
                        if (g_lobby_page > 1) g_lobby_page--;
                    }

                    // navigate upon selecting
                    else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        if ((g_lobby_page - 1) * 5 < g_lobby_search_result_length) {
                            g_view_item_id = g_lobby_search_result[(g_lobby_page - 1) * 5];
                            s_scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        if ((g_lobby_page - 1) * 5 + 1 < g_lobby_search_result_length) {
                            g_view_item_id = g_lobby_search_result[(g_lobby_page - 1) * 5 + 1];
                            s_scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        if ((g_lobby_page - 1) * 5 + 2 < g_lobby_search_result_length) {
                            g_view_item_id = g_lobby_search_result[(g_lobby_page - 1) * 5 + 2];
                            s_scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        if ((g_lobby_page - 1) * 5 + 3 < g_lobby_search_result_length) {
                            g_view_item_id = g_lobby_search_result[(g_lobby_page - 1) * 5 + 3];
                            s_scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        if ((g_lobby_page - 1) * 5 + 4 < g_lobby_search_result_length) {
                            g_view_item_id = g_lobby_search_result[(g_lobby_page - 1) * 5 + 4];
                            s_scene = 5;
                            lobby_reset();
                        }
                    } else if (IsKeyPressed(KEY_A)) {
                        g_lobby_filters[0] = !g_lobby_filters[0];
                        lobby_sync_lost_item_list();
                    } else if (IsKeyPressed(KEY_W)) {
                        g_lobby_filters[1] = !g_lobby_filters[1];
                        lobby_sync_lost_item_list();
                    } else if (IsKeyPressed(KEY_C)) {
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_P)) {
                        s_scene = 4;
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_D)) {
                        s_scene = 6;
                        DM_reset();
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_L)) {
                        s_scene = 0;
                        lobby_reset();
                    } else if (IsKeyPressed(KEY_N)) {
                        s_scene = 9;
                        notif_reset();
                        lobby_reset();
                    }
                }
                break;

            case 4:  // post
                extern int g_post_selected;
                extern bool g_post_tags_selected[6];
                extern char g_post_title[MAX_TITLE_LEN];
                extern char g_post_content[MAX_CONTENT_LEN];
                extern char g_post_target_user[MAX_USERNAME_LEN];
                extern char g_post_school_number[MAX_USERNAME_LEN];

                post_screen();
                strcpy(g_post_school_number, g_school_number);

                if (g_typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        g_typing = false;
                        g_post_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (g_post_selected == 1 && strlen(g_post_title) > 0) g_post_title[strlen(g_post_title) - 1] = '\0';
                        else if (g_post_selected == 2 && strlen(g_post_content) > 0) g_post_content[strlen(g_post_content) - 1] = '\0';
                        else if (g_post_selected == 3 && strlen(g_post_target_user) > 0) g_post_target_user[strlen(g_post_target_user) - 1] = '\0';
                    } else {
                        // todo: word count limit
                        if (g_post_selected == 1 && strlen(g_post_title) < MAX_TITLE_LEN - 1)
                            g_post_title[strlen(g_post_title)] = GetCharPressed();
                        else if (g_post_selected == 2 && strlen(g_post_content) < MAX_CONTENT_LEN - 1)
                            g_post_content[strlen(g_post_content)] = GetCharPressed();
                        else if (g_post_selected == 3 && strlen(g_post_target_user) < MAX_USERNAME_LEN - 1)
                            g_post_target_user[strlen(g_post_target_user)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_T)) {
                        g_typing = true;
                        g_post_selected = 1;
                    } else if (IsKeyPressed(KEY_C)) {
                        g_typing = true;
                        g_post_selected = 2;
                    } else if (IsKeyPressed(KEY_U)) {
                        g_typing = true;
                        g_post_selected = 3;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        g_post_tags_selected[0] = !g_post_tags_selected[0];
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        g_post_tags_selected[1] = !g_post_tags_selected[1];
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        g_post_tags_selected[2] = !g_post_tags_selected[2];
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        g_post_tags_selected[3] = !g_post_tags_selected[3];
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        g_post_tags_selected[4] = !g_post_tags_selected[4];
                    } else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                        g_post_tags_selected[5] = !g_post_tags_selected[5];
                    } else if (IsKeyPressed(KEY_P)) {
                        if (post_valid()) {
                            g_lost_items->insert_lost_item(g_lost_items, g_post_title, g_post_content, g_post_target_user, g_post_school_number, g_post_tags_selected, false);
                            save_new_lost_item(g_lost_items->list[g_lost_items->len - 1]);
                            post_reset();
                            s_scene = 3;
                        }
                    } else if (IsKeyPressed(KEY_B)) {
                        post_reset();
                        s_scene = 3;
                    }
                }
                break;

            case 5:  // view
                extern LostItem* view_item;
                extern char g_view_school_number[MAX_USERNAME_LEN];
                extern bool g_view_sync;
                view_item = g_lost_items->list[g_view_item_id];


                strcpy(g_view_school_number, g_school_number);

                if (!g_view_sync) {
                    view_synchronize();
                    g_view_sync = true;
                }

                view_screen();

                if (IsKeyPressed(KEY_B)) {
                    s_scene = 3;
                    view_reset();
                } else if (IsKeyPressed(KEY_D) && strcmp(view_item->writer, g_school_number) != 0) {
                    // todo: directly to DM
                    strcpy(g_DMsend_receiver, view_item->writer);
                    s_scene = 8;
                } else if (IsKeyPressed(KEY_M) && strcmp(view_item->writer, g_school_number) == 0) {
                    // mark as deleted
                    g_lost_items->delete_lost_item(g_lost_items, g_view_item_id);
                    view_reset();
                    s_scene = 3;
                }
                break;

            case 6:  // DM
                extern int g_dm_selected;
                extern bool g_DM_sync;
                extern char g_DM_school_number[50];
                extern int g_DM_types;
                extern int g_dm_search_result_length;
                extern int* g_dm_search_result;
                extern int g_DM_page;

                strcpy(g_DM_school_number, g_school_number);

                if (!g_DM_sync) {
                    DMListSync();
                    g_DM_sync = true;
                }

                DMscreen();

                if (!g_typing) {
                    if (IsKeyPressed(KEY_S)) {
                        g_DM_types = 1;
                        DMListSync();
                    }
                    else if (IsKeyPressed(KEY_I)) {
                        g_DM_types = 2;
                        DMListSync();
                    } else if (IsKeyPressed(KEY_B)) {
                        DM_reset();
                        s_scene = 3;
                    }

                    else if (IsKeyPressed(KEY_RIGHT)) {
                        if (g_DM_page < (g_dm_search_result_length / 7) + (g_dm_search_result_length % 7 ? 1 : 0)) g_DM_page++;
                    } else if (IsKeyPressed(KEY_LEFT)) {
                        if (g_DM_page > 1) g_DM_page--;
                    } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                        if ((g_DM_page - 1) * 5 < g_dm_search_result_length) {
                            g_viewDM_id = g_dm_search_result[(g_DM_page - 1) * 7];
                            s_scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                        if ((g_DM_page - 1) * 7 + 1 < g_dm_search_result_length) {
                            g_viewDM_id = g_dm_search_result[(g_DM_page - 1) * 7 + 1];
                            s_scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                        if ((g_DM_page - 1) * 7 + 2 < g_dm_search_result_length) {
                            g_viewDM_id = g_dm_search_result[(g_DM_page - 1) * 7 + 2];
                            s_scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                        if ((g_DM_page - 1) * 7 + 3 < g_dm_search_result_length) {
                            g_viewDM_id = g_dm_search_result[(g_DM_page - 1) * 7 + 3];
                            s_scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                        if ((g_DM_page - 1) * 7 + 4 < g_dm_search_result_length) {
                            g_viewDM_id = g_dm_search_result[(g_DM_page - 1) * 7 + 4];
                            s_scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                        if ((g_DM_page - 1) * 7 + 5 < g_dm_search_result_length) {
                            g_viewDM_id = g_dm_search_result[(g_DM_page - 1) * 7 + 5];
                            s_scene = 7;
                            DM_reset();
                        }
                    } else if (IsKeyPressed(KEY_SEVEN) || IsKeyPressed(KEY_KP_7)) {
                        if ((g_DM_page - 1) * 7 + 6 < g_dm_search_result_length) {
                            g_viewDM_id = g_dm_search_result[(g_DM_page - 1) * 7 + 6];
                            s_scene = 7;
                            DM_reset();
                        }
                    }
                }
                break;

            case 7:  // DM view
                extern DMMessage* g_viewDM;
                extern int g_from_notif;

                printf("DEBUG: viewDMId = %d, dmMessages->len = %d\n", g_viewDM_id, g_dm_messages->len);

                g_viewDM = g_dm_messages->list[g_viewDM_id];
                DMview_screen();

                if (!g_typing) {
                    if (IsKeyPressed(KEY_B)) {
                        DMview_reset();
                        if (g_from_notif) {
                            s_scene = 9;
                            g_from_notif = 0;
                        } else {
                            s_scene = 6;
                        }
                    }
                }
                break;

            case 8: //DM send
                extern int g_DMsend_selected;
                extern char g_DMsend_title[MAX_TITLE_LEN];
                extern char g_DMsend_content[MAX_CONTENT_LEN];
                extern char g_DMsend_receiver[MAX_USERNAME_LEN];
                extern char g_DMsend_school_number[MAX_USERNAME_LEN];

                strcpy(g_DMsend_school_number, g_school_number);
                
                DMsend_screen();

                if (g_typing) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        g_typing = false;
                        g_DMsend_selected = 0;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (g_DMsend_selected == 1 && strlen(g_DMsend_title) > 0)
                            g_DMsend_title[strlen(g_DMsend_title) - 1] = '\0';
                        else if (g_DMsend_selected == 2 && strlen(g_DMsend_content) > 0)
                            g_DMsend_content[strlen(g_DMsend_content) - 1] = '\0';
                    } else {
                        // todo: word count limit
                        if (g_DMsend_selected == 1 && strlen(g_DMsend_title) < MAX_TITLE_LEN - 1)
                            g_DMsend_title[strlen(g_DMsend_title)] = GetCharPressed();
                        else if (g_DMsend_selected == 2 && strlen(g_DMsend_content) < MAX_CONTENT_LEN - 1)
                            g_DMsend_content[strlen(g_DMsend_content)] = GetCharPressed();
                    }
                } else {
                    if (IsKeyPressed(KEY_B)) {
                        DMview_reset();
                        s_scene = 5;
                    } else if (IsKeyPressed(KEY_T)) {
                        g_typing = true;
                        g_DMsend_selected = 1;
                    } else if (IsKeyPressed(KEY_C)) {
                        g_typing = true;
                        g_DMsend_selected = 2;
                    } else if (IsKeyPressed(KEY_P)) {
                        if (DMsend_valid()) {
                            g_dm_messages->insert_message(g_dm_messages, g_DMsend_title, g_DMsend_content, g_DMsend_school_number, g_DMsend_receiver);
                            save_new_dm(g_dm_messages->list[g_dm_messages->len - 1]);
                            DMsend_reset();
                            s_scene = 5;
                        }
                    }
                }
                break;

            case 9:  // notifications
                extern int g_notif_sellected;
                extern char g_notif_lines[MAX_LINES][MAX_CONTENT_LEN];
                extern int g_notif_count;
                extern int g_notif_total_page;
                extern int g_current_page;
                extern char g_id[LINES_PER_PAGE][3][20];
                extern int g_DM_types;
                extern int g_from_notif;

                strcpy(g_DM_school_number, g_school_number);

                if (g_notif_sellected == 1) {
                    DMListSync();
                    read_notif_file();
                    g_notif_sellected = 0;
                }

                DrawText(TextFormat("notif_count: %d", g_notif_count), 20, HEIGHT - 120, 30, BLACK);
                DrawText(TextFormat("notif_total_page: %d", g_notif_total_page), 20, HEIGHT - 160, 30, BLACK);
                DrawText(TextFormat("current_page: %d", g_current_page), 20, HEIGHT - 200, 30, BLACK);

                notif_screen();

                if (IsKeyPressed(KEY_B)) {
                    s_scene = 3;
                } else if (IsKeyPressed(KEY_C)) {
                    clear_notif();
                    read_notif_file();
                    g_notif_count = 0;
                    g_notif_total_page = 1;
                    g_current_page = 1;
                } else if (IsKeyPressed(KEY_RIGHT)) {
                    if (g_current_page < g_notif_total_page) g_current_page++;
                    read_notif_file();
                } else if (IsKeyPressed(KEY_LEFT)) {
                    if (g_current_page > 1) g_current_page--;
                    read_notif_file();
                } else if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                    g_DM_types = 2;
                    read_notif_file();
                    if ((g_current_page - 1) * 5 < g_notif_count) {
                        g_viewDM_id = g_dm_messages->search_dm_by_elements(g_dm_messages, g_id[0][0], g_id[0][1], atoi(g_id[0][2]));
                        notif_reset();
                        s_scene = 7;
                        g_from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                    g_DM_types = 2;
                    read_notif_file();
                    if ((g_current_page - 1) * 5 + 1 < g_notif_count) {
                        g_viewDM_id = g_dm_messages->search_dm_by_elements(g_dm_messages, g_id[1][0], g_id[1][1], atoi(g_id[1][2]));
                        notif_reset();
                        s_scene = 7;
                        g_from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                    g_DM_types = 2;
                    read_notif_file();
                    if ((g_current_page - 1) * 5 + 2 < g_notif_count) {
                        g_viewDM_id = g_dm_messages->search_dm_by_elements(g_dm_messages, g_id[2][0], g_id[2][1], atoi(g_id[2][2]));
                        notif_reset();
                        s_scene = 7;
                        g_from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                    g_DM_types = 2;
                    read_notif_file();
                    if ((g_current_page - 1) * 5 + 3 < g_notif_count) {
                        g_viewDM_id = g_dm_messages->search_dm_by_elements(g_dm_messages, g_id[3][0], g_id[3][1], atoi(g_id[3][2]));
                        notif_reset();
                        s_scene = 7;
                        g_from_notif = 1;
                    }
                } else if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                    g_DM_types = 2;
                    read_notif_file();
                    if ((g_current_page - 1) * 5 + 4 < g_notif_count) {
                        g_viewDM_id = g_dm_messages->search_dm_by_elements(g_dm_messages, g_id[4][0], g_id[4][1], atoi(g_id[4][2]));
                        notif_reset();
                        s_scene = 7;
                        g_from_notif = 1;
                    }
                }
                break;
            
           
            
        }

        EndDrawing();
    }

    CloseWindow();

    for (int i = 0; i < g_lost_items->len; i++) {
        free(g_lost_items->list[i]);
        g_lost_items->list[i] = NULL;
    }
    free(g_lost_items);
    return 0;
}
