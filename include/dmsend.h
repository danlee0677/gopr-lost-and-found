#ifndef DMSEND_H
#define DMSEND_H

#include "../include/models.h"

extern char DMsendTitle[MAX_TITLE_LEN];
extern char DMsendContent[MAX_CONTENT_LEN];
extern int DMsendSelected;
extern char DMsendReceiver[MAX_USERNAME_LEN];

void DMsendScreen();
void DMsendReset();
bool DMsendValid();

#endif

// 고프 뺑이 처라 ㅋㅋ