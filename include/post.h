#ifndef POST_H
#define POST_H

#include <models.h>

extern char g_post_title[MAX_TITLE_LEN];
extern char g_post_content[MAX_CONTENT_LEN];
extern int g_post_selected;
extern bool g_post_tags_selected[6];
extern char g_post_target_user[MAX_USERNAME_LEN];

void post_screen();
void post_reset();
bool post_valid();

#endif