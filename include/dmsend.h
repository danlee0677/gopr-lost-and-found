#ifndef DMSEND_H
#define DMSEND_H

#include "../include/models.h"

extern char g_DMsend_title[MAX_TITLE_LEN]; // 보내는 DM 제목
extern char g_DMsend_content[MAX_CONTENT_LEN]; // 보내는 DM 본문 내용
extern int g_DMsend_selected; // 보내는 DM 받는 사람
extern char g_DMsend_receiver[MAX_USERNAME_LEN]; // 보내는 DM 보내는 사람

void DMsend_screen(); // DM 보내는 화면 띄우기
void DMsend_reset(); // DM 보내는 화면 닫기
bool DMsend_valid(); // DM 보낼 때 형식이 맞는 지 체크하기

#endif

// 고프 뺑이 처라 ㅋㅋ