#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/models.h"

void load_lost_item_list(LostItemList *list) {
    FILE *fptr = fopen("./data/lostitems.txt", "r");
    if (fptr == NULL) {
        printf("Failed to load lost_item_list\n");
        return;
    }
    char readTitle[MAX_TITLE_LEN];
    char readContent[MAX_CONTENT_LEN];
    int tempReadDeleted;
    bool readDeleted;
    char readTargetUser[MAX_USERNAME_LEN];
    char readTags[8];
    bool tags[6] = {false, false, false, false, false, false};
    while (fscanf(fptr, "%s %s %d %s %s", readTitle, readContent, &tempReadDeleted, readTargetUser, readTags) != EOF) {
        for (int i = 0; i < 6; i++) tags[i] = (bool)(readTags[i] - '0');
        for (int i = 0; i < strlen(readTitle); i++) {
            if (readTitle[i] == '_') readTitle[i] = ' ';
        }
        for (int i = 0; i < strlen(readContent); i++) {
            if (readContent[i] == '_') readContent[i] = ' ';
        }
        readDeleted = (bool)tempReadDeleted;
        list->insert_lost_item(list, readTitle, readContent, readTargetUser, tags, readDeleted);
        printf("%s\n", readContent);
    }

    for (int i = 0; i < list->len; i++) {
        LostItem *temp = list->list[i];
        printf("%s\n", temp->content);
    }

    fclose(fptr);
}

void save_new_lost_item(LostItem *item) {
    FILE *fptr = fopen("./data/lostitems.txt", "a+");
    if (fptr == NULL) {
        printf("Failed to save new lost item\n");
        return;
    }
    char saveTitle[MAX_TITLE_LEN];
    char saveContent[MAX_CONTENT_LEN];
    strcpy(saveTitle, item->title);
    strcpy(saveContent, item->content);
    for (int i = 0; i < strlen(item->title); i++) {
        if (saveTitle[i] == ' ') saveTitle[i] = '_';
    }
    for (int i = 0; i < strlen(item->content); i++) {
        if (saveContent[i] == ' ') saveContent[i] = '_';
    }
    char saveTag[7];
    for (int i = 0; i < 6; i++) saveTag[i] = item->tags[i] + '0';
    saveTag[6] = '\0';
    fprintf(fptr, "%s %s %d %s %s\n", saveTitle, saveContent, (int)item->deleted, item->target_user, saveTag);

    fclose(fptr);
}

// 분실물 삽입 함수
// 동적 배열 크기를 초과하면 크기를 2배로 확장
void lost_item_list_insert_lost_item(LostItemList *self, char title[], char content[], char target_user[], bool tags[], bool deleted = false) {
    if (self->len < self->max_len) {
        LostItem *new_item = (LostItem *)malloc(sizeof(LostItem));
        strcpy(new_item->content, content);
        strcpy(new_item->title, title);
        strcpy(new_item->target_user, target_user);
        new_item->_id = self->len;
        for (int i = 0; i < 6; i++) {
            new_item->tags[i] = tags[i];
        }
        new_item->deleted = deleted;
        self->list[self->len++] = new_item;
    } else {
        self->max_len *= 2; // 배열 크기 확장
        self->list = (LostItem **)realloc(self->list, sizeof(LostItem *) * self->max_len);
        lost_item_list_insert_lost_item(self, title, content, target_user, tags); // 재귀 호출로 삽입 재시도
    }
}

// 분실물 삭제 함수
// ID 범위만 체크하며 삭제 플래그 처리
void lost_item_list_delete_lost_item(LostItemList *self, int _id) {
    if (_id < self->max_len) {
        self->list[_id]->deleted = true;
    }
}

// 태그를 기반으로 검색하여 해당되는 index 배열 반환
// 반환된 배열은 -1로 종료되며, 사용 후 반드시 free
int* lost_item_list_search_tag(LostItemList *self, bool tags[]) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    for (int i = 0; i < self->len; i++) {
        bool flag = true;
        for (int j = 0; j < 6; j++) {
            if (self->list[i]->deleted || (tags[j] && !self->list[i]->tags[j])) {
                flag = false;
                break;
            }
        }
        if (flag) ret[len++] = i;
    }
    ret[len] = -1; // 종료 표시
    return ret;
}

// 키워드를 기반으로 제목과 내용에서 검색
// 대상 유저가 없는 게시물(target_user == -1)만 검색 대상
int* lost_item_list_search_content(LostItemList *self, char **keywords, int keyword_count) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    for (int i = 0; i < self->len; i++) {
        if (self->list[i]->deleted || strcmp(self->list[i]->target_user, "") != 0) continue;
        for (int j = 0; j < keyword_count; j++) {
            if (strstr(self->list[i]->title, keywords[j]) || strstr(self->list[i]->content, keywords[j])) {
                ret[len++] = i;
                break;
            }
        }
    }
    ret[len] = -1;
    return ret;
}

// 유저 ID를 기반으로 본인이 지정된 분실물 검색
int *lost_item_list_search_user_id(LostItemList *self, char _id[]) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    for (int i = 0; i < self->len; i++) {
        if (!self->list[i]->deleted && strcmp(self->list[i]->target_user, _id) == 0) ret[len++] = i;
    }
    ret[len] = -1;
    return ret;
}

// 분실물 리스트 생성 함수
// 메서드 포인터 및 초기 배열 설정
LostItemList* create_lost_item_list() {
    LostItemList *ret = (LostItemList *)malloc(sizeof(LostItemList));
    ret->delete_lost_item = lost_item_list_delete_lost_item;
    ret->insert_lost_item = lost_item_list_insert_lost_item;
    ret->search_content = lost_item_list_search_content;
    ret->search_tag = lost_item_list_search_tag;
    ret->search_user_id = lost_item_list_search_user_id;
    ret->len = 0;
    ret->max_len = 1;
    ret->list = (LostItem **)malloc(sizeof(LostItem *));
    return ret;
}

LostItemList *lostItems = create_lost_item_list();