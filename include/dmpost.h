#ifndef DMPOST_H
#define POST_H

#include "../include/models.h"

extern char postTitle[MAX_TITLE_LEN];
extern char postContent[MAX_CONTENT_LEN];
extern int postSelected;
extern bool postTagsSelected[6];
extern char postTargetUser[MAX_USERNAME_LEN];

void PostScreen();
void PostReset();
bool PostValid();

#endif