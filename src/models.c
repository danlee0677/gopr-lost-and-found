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
        char readTempUser[1] = "";
        list->insert_lost_item(list, readTitle, readContent, (strcmp(readTargetUser, "00000") == 0 ? readTempUser : readTargetUser), tags, readDeleted);
    }

    for (int i = 0; i < list->len; i++) {
        LostItem *temp = list->list[i];
        printf("%s\n", temp->target_user);
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
    char saveUserTemp[6] = "00000"; // to prevent missing target user when loading & saving
    fprintf(fptr, "%s %s %d %s %s\n", saveTitle, saveContent, (int)item->deleted, (strlen(item->target_user) == 0 ? saveUserTemp : item->target_user), saveTag);

    fclose(fptr);
}

// 분실물 삽입 함수
// 동적 배열 크기를 초과하면 크기를 2배로 확장
void lost_item_list_insert_lost_item(LostItemList *self, char title[], char content[], char target_user[], bool tags[], bool deleted) {
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
        lost_item_list_insert_lost_item(self, title, content, target_user, tags, deleted); // 재귀 호출로 삽입 재시도
    }
}

// 분실물 삭제 함수
// ID 범위만 체크하며 삭제 플래그 처리
void lost_item_list_delete_lost_item(LostItemList *self, int _id) {
    if (_id < self->max_len) {
        self->list[_id]->deleted = true;
    }
}

// 태그 및 내용물을 기반으로 검색하여 해당되는 index 배열 반환
// 반환된 배열은 -1로 종료되며, 사용 후 반드시 free
int* lost_item_list_search(LostItemList *self, char keyword[], bool tags[], char _id[]) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    bool tags_exist = false, _id_exist = (strlen(_id) == 0 ? false : true), keyword_exist = (strlen(keyword) == 0 ? false : true);
    for (int i = 0; i < 6; i++) if (tags[i]) tags_exist = true;

    for (int i = 0; i < self->len; i++) {
        bool flag = true;
        // already deleted
        if (self->list[i]->deleted) {
            flag = false; break;
        }
        // search for tags
        if (tags_exist) {
            for (int j = 0; j < 6; j++) {
                if (tags[j] && !self->list[i]->tags[j]) {
                    flag = false;
                    break;
                }
            }
        }
        // search for matching tags
        if (_id_exist) {
            if (!strcmp(self->list[i]->target_user, _id) != 0) flag = false;
        }
        // search for keyword matches
        if (keyword_exist) {
            if (strstr(self->list[i]->title, keyword) == NULL && strstr(self->list[i]->content, keyword) == NULL) {
                flag = false;
            }
        }
        if (flag) ret[len++] = i;
    }

    ret[len] = -1; // 종료 표시
    return ret;
}

// 분실물 리스트 생성 함수
// 메서드 포인터 및 초기 배열 설정
LostItemList* create_lost_item_list() {
    LostItemList *ret = (LostItemList *)malloc(sizeof(LostItemList));
    ret->delete_lost_item = lost_item_list_delete_lost_item;
    ret->insert_lost_item = lost_item_list_insert_lost_item;
    ret->search_lost_item = lost_item_list_search;
    ret->len = 0;
    ret->max_len = 1;
    ret->list = (LostItem **)malloc(sizeof(LostItem *));
    return ret;
}

int lost_item_list_length(int *ind) {
    int ret = 0;
    while (ind[ret] != -1) ret++;
    return ret;
}

LostItemList *lostItems = create_lost_item_list();