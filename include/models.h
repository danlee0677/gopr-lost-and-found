#ifndef MODELS_H
#define MODELS_H

#include <stdbool.h>
#include "../include/constants.h"

typedef enum {
    ELECTRONICS, // 전자기기
    CLOTHINGS, // 옷
    BAGS, // 가방
    STATIONERY, // 학용품
    PERSONAL, // 개인물품: 지갑, 신분증 등
    SPECIAL, // 기타 분류
} TagEnum;

typedef struct {
    int _id;                      // 게시글 고유 ID
    char title[MAX_TITLE_LEN];   // 게시글 제목
    char content[MAX_CONTENT_LEN]; // 게시글 내용
    bool deleted;                // 삭제 여부
    char target_user[MAX_USERNAME_LEN];             // 분실물 주인(없으면 -1)
    bool tags[6];             // 태그
} LostItem;

typedef struct LostItemList_t {
    void (*insert_lost_item)(struct LostItemList_t* self, char title[], char content[], char target_user[], bool tags[], bool deleted); // 분실물 삽입
    void (*delete_lost_item)(struct LostItemList_t* self, int _id);   // 분실물 삭제
    int* (*search_tag)(struct LostItemList_t* self, bool tags[]);     // 태그 검색
    int* (*search_content)(struct LostItemList_t* self, char **keywords, int keyword_count); // 내용 검색
    int* (*search_user_id)(struct LostItemList_t *self, char _id[]);     // 유저 ID 검색
    LostItem **list;        // 분실물 배열
    int len, max_len;       // 현재 길이, 최대 길이
} LostItemList;

extern LostItemList *lostItems;

void load_lost_item_list(LostItemList *list);
void save_new_lost_item(LostItem *item);

#endif