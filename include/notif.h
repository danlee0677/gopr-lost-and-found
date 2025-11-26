#ifndef NOTIF_H
#define NOTIF_H

#define MAX_LINES 1024
#define FILE_NAME_MAX_LEN 100
#define LINES_PER_PAGE 5

void notif_screen(); // 알림창 화면 그리기
void read_notif_file(); // 알림 파일 읽기
void clear_notif(); // 알림 파일 비우기
void notif_reset(); // 알림창 초기화

#endif