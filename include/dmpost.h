#ifndef DMPOST_H
#define DMPOST_H

#include "../include/models.h"

extern char DMpostTitle[MAX_TITLE_LEN];
extern char DMpostContent[MAX_CONTENT_LEN];
extern int DMpostSelected;
extern char DMpostReceiver[MAX_USERNAME_LEN];

void DMPostScreen();
void DMPostReset();
bool DMPostValid();

#endif