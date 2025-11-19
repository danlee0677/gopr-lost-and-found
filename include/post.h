#ifndef POST_H
#define POST_H

#include <models.h>

extern char postSchoolNumber[50];
extern char postTitle[MAX_TITLE_LEN];
extern char postContent[MAX_CONTENT_LEN];
extern int postSelected;
extern bool postTagsSelected[6];
extern char postTargetUser[MAX_USERNAME_LEN];

void PostScreen();
void PostReset();
bool PostValid();

#endif