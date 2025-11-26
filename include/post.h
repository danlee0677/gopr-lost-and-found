#ifndef POST_H
#define POST_H

#include <models.h>

extern char g_post_title[MAX_TITLE_LEN];
extern char g_post_content[MAX_CONTENT_LEN];
extern int g_post_selected;
extern bool g_post_tags_selected[6];
extern char g_post_target_user[MAX_USERNAME_LEN];

void post_screen(); // post 시 화면
void post_reset(); // post 리셋용
bool post_valid(); // 작성한 게시글이 유효한지 확인

#endif