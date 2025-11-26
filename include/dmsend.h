#ifndef DMSEND_H
#define DMSEND_H

#include "../include/models.h"

extern char DMsend_title[MAX_TITLE_LEN];
extern char DMsend_content[MAX_CONTENT_LEN];
extern int DMsend_selected;
extern char DMsend_receiver[MAX_USERNAME_LEN];

void DMsend_screen();
void DMsend_reset();
bool DMsend_valid();

#endif

// 고프 뺑이 처라 ㅋㅋ