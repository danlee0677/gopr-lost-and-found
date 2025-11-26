#ifndef DMSEND_H
#define DMSEND_H

#include "../include/models.h"

extern char g_DMsend_title[MAX_TITLE_LEN];
extern char g_DMsend_content[MAX_CONTENT_LEN];
extern int g_DMsend_selected;
extern char g_DMsend_receiver[MAX_USERNAME_LEN];

void DMsend_screen();
void DMsend_reset();
bool DMsend_valid();

#endif

// 고프 뺑이 처라 ㅋㅋ