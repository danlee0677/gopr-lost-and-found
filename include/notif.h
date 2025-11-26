#ifndef NOTIF_H
#define NOTIF_H

#define MAX_LINES 1024
#define FILE_NAME_MAX_LEN 100
#define LINES_PER_PAGE 5

void notif_screen();
void read_notif_file();
void clear_notif();
void notif_reset();

#endif