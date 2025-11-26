#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/models.h"
#include "../include/raylib.h"

void load_lost_item_list(LostItemList *list) {
    FILE *fptr = fopen("./data/lostitems.txt", "r");
    if (fptr == NULL) {
        printf("Failed to load lost_item_list\n");
        return;
    }
    char read_title[MAX_TITLE_LEN];
    char read_content[MAX_CONTENT_LEN];
    int temp_read_deleted;
    bool read_deleted;
    char read_target_user[MAX_USERNAME_LEN];
    char read_tags[8];
    char read_writer[MAX_USERNAME_LEN];
    bool tags[6] = {false, false, false, false, false, false};
    while (fscanf(fptr, "%s %s %d %s %s %s", read_title, read_content, &temp_read_deleted, read_target_user, read_tags, read_writer) != EOF) {
        for (int i = 0; i < 6; i++) tags[i] = (bool)(read_tags[i] - '0');
        for (int i = 0; i < strlen(read_title); i++) {
            if (read_title[i] == '_') read_title[i] = ' ';
        }
        for (int i = 0; i < strlen(read_content); i++) {
            if (read_content[i] == '_') read_content[i] = ' ';
        }
        read_deleted = (bool)temp_read_deleted;
        char read_temp_user[1] = "";
        list->insert_lost_item(list, read_title, read_content, (strcmp(read_target_user, "00000") == 0 ? read_temp_user : read_target_user), read_writer, tags, read_deleted);
    }

    for (int i = 0; i < list->len; i++) {
        LostItem *temp = list->list[i];
    }

    fclose(fptr);
}

void save_new_lost_item(LostItem *item) {
    FILE *fptr = fopen("./data/lostitems.txt", "a+");
    if (fptr == NULL) {
        printf("Failed to save new lost item\n");
        return;
    }
    char save_title[MAX_TITLE_LEN];
    char save_content[MAX_CONTENT_LEN];
    strcpy(save_title, item->title);
    strcpy(save_content, item->content);
    for (int i = 0; i < strlen(item->title); i++) {
        if (save_title[i] == ' ') save_title[i] = '_';
    }
    for (int i = 0; i < strlen(item->content); i++) {
        if (save_content[i] == ' ') save_content[i] = '_';
    }
    char save_tag[7];
    for (int i = 0; i < 6; i++) save_tag[i] = item->tags[i] + '0';
    save_tag[6] = '\0';
    char save_user_temp[6] = "00000"; // to prevent missing target user when loading & saving
    fprintf(fptr, "%s %s %d %s %s %s\n", save_title, save_content, (int)item->deleted, (strlen(item->target_user) == 0 ? save_user_temp : item->target_user), save_tag, item->writer);

    fclose(fptr);
}

// 분실물 삽입 함수
// 동적 배열 크기를 초과하면 크기를 2배로 확장
void lost_item_list_insert_lost_item(LostItemList *self, char title[], char content[], char target_user[], char writer[], bool tags[], bool deleted) {
    if (self->len < self->max_len) {
        LostItem *new_item = (LostItem *)malloc(sizeof(LostItem));
        strcpy(new_item->content, content);
        strcpy(new_item->title, title);
        strcpy(new_item->target_user, target_user);
        strcpy(new_item->writer, writer);
        new_item->_id = self->len;
        for (int i = 0; i < 6; i++) {
            new_item->tags[i] = tags[i];
        }
        new_item->deleted = deleted;
        self->list[self->len++] = new_item;
    } else {
        self->max_len *= 2; // 배열 크기 확장
        self->list = (LostItem **)realloc(self->list, sizeof(LostItem *) * self->max_len);
        lost_item_list_insert_lost_item(self, title, content, target_user, writer, tags, deleted); // 재귀 호출로 삽입 재시도
    }
}

// 분실물 삭제 함수
// ID 범위만 체크하며 삭제 플래그 처리
void lost_item_list_delete_lost_item(LostItemList *self, int _id) {
    FILE *fptr = fopen("./data/lostitems.txt", "r+");
    
    if (fptr == NULL) return;
    
    char temp;
    int cnt = 0;
    // seek to the line where the item lives
    while (cnt < _id && (temp = fgetc(fptr)) != EOF) {
        if (temp == '\n') cnt++;
    }
    // seek until right before the line
    cnt = 0;
    while (cnt < 2 && (temp = fgetc(fptr)) != EOF) {
        if (temp == ' ') cnt++;
    }
    fseek(fptr, 0, SEEK_CUR);
    fprintf(fptr, "1");

    if (_id < self->max_len) {
        self->list[_id]->deleted = true;
    }
    fclose(fptr);
}

// 태그 및 내용물을 기반으로 검색하여 해당되는 index 배열 반환
// 반환된 배열은 -1로 종료되며, 사용 후 반드시 free
// _id: 현재 로그인된 유저
int* lost_item_list_search(LostItemList *self, char keyword[], bool tags[], char _id[], bool filters[]) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    bool tags_exist = false, keyword_exist = (strlen(keyword) == 0 ? false : true);
    for (int i = 0; i < 6; i++) if (tags[i]) tags_exist = true;
    for (int i = 0; i < self->len; i++) {
        bool flag = true;
        // first filter: addressed to me
        if (filters[0] && strcmp(self->list[i]->target_user, _id) != 0) {
            flag = false;
        }
        // second filter: I am the writer
        if (filters[1] && strcmp(self->list[i]->writer, _id) != 0) {
            flag = false;
        }
        // already deleted
        if (self->list[i]->deleted) {
            flag = false;
        }
        // has targeted user, but is not the current user
        if (strlen(self->list[i]->target_user) != 0 && strcmp(self->list[i]->target_user, _id) != 0) {
            flag = false;
        }
        // search for tags
        if (tags_exist) {
            for (int j = 0; j < 6; j++) {
                if (tags[j] && !self->list[i]->tags[j]) {
                    flag = false;
                }
            }
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

LostItemList *lost_items = create_lost_item_list();

/*---------------------채팅---------------------*/
//채팅 리스트에 메세지 추가
void load_dm_list(DMList *list) {
    list->len = 0;

        list->list = (DMMessage **)malloc(sizeof(DMMessage *) * list->max_len);
    if (list->list == NULL) {
        fprintf(stderr, "load_dm_list: malloc failed\n");
        list->max_len = 0;
        return;
    }

    // data/dm 폴더의 파일들 로드
    FilePathList files = LoadDirectoryFiles("data/dm");
    printf("CWD = %s\n", GetWorkingDirectory());
    printf("DM file count = %d\n", files.count);

    for (int i = 0; i < files.count; i++) {
        const char *full_path = files.paths[i];
        const char *filename = GetFileName(full_path);

        // 파일 이름에서 sender / receiver / id 파싱
        char sender[50];
        char receiver[50];
        int fileId; // 필요하면 나중에 DMMessage->id에 쓸 수도 있음

        // "sender receiver id.txt" 형식이라고 가정
        // .txt 포함된 상태로 sscanf 해도 동작함 (뒤에 .txt는 무시됨)
        if (sscanf(filename, "%49s %49s %d", sender, receiver, &fileId) != 3) {
            printf("skip invalid filename: %s\n", filename);
            continue;
        }

        // 파일 열기
        FILE *fp = fopen(full_path, "r");
        if (!fp) {
            fprintf(stderr, "failed to open %s\n", full_path);
            continue;
        }

        char title[MAX_TITLE_LEN];
        char content[MAX_MESSAGE_LEN];
        char line[512];

        // 첫 줄: title
        if (!fgets(title, sizeof(title), fp)) {
            fclose(fp);
            continue;
        }
        // 개행 제거
        title[strcspn(title, "\r\n")] = '\0';

        // 나머지 줄들: content
        content[0] = '\0';
        while (fgets(line, sizeof(line), fp)) {
            size_t curLen = strlen(content);
            size_t lineLen = strlen(line);

            if (curLen + lineLen + 1 >= sizeof(content)) {
                // 더 이상 넣을 공간 없으면 잘라서 넣고 종료
                size_t avail = sizeof(content) - curLen - 1;
                if (avail > 0) {
                    strncat(content, line, avail);
                }
                content[sizeof(content) - 1] = '\0';
                break;
            } else {
                strcat(content, line);
            }
        }

        fclose(fp);

        // DMList에 추가
        list->insert_message(list, title, content, sender, receiver);
        list->list[list->len - 1]->id = fileId;
    }

    UnloadDirectoryFiles(files);
}

// DM 저장
void save_new_dm(DMMessage *item) {
    char path[256];
    snprintf(path, sizeof(path), "./data/dm/%s %s %d.txt", item->sender, item->receiver, item->id);
    printf("%s %s", item->sender, item->receiver);

    FILE *fptr = fopen(path, "w");
    if (fptr == NULL) {
        printf("Failed to save new DM: %s\n", path);
        return;
    }

    // 1줄: title
    fprintf(fptr, "%s\n", item->title);

    // 2줄부터: content (여러 줄 포함 가능)
    fprintf(fptr, "%s", item->content);

    fclose(fptr);

    fptr = fopen("./data/notif.txt", "a+");
    if (fptr == NULL) {
        printf("Failed to save new notification\n");
        return;
    }
    fprintf(fptr, "%s %s %d\n", item->sender, item->receiver, item->id);
    fclose(fptr);
}

// 채팅 리스트에 채팅 추가
void dm_list_insert_message(DMList *self, char title[], char content[], char sender[], char receiver[]) {
    if (self->len < self->max_len) {
        DMMessage *new_item = (DMMessage *)malloc(sizeof(DMMessage));
        strcpy(new_item->content, content);
        strcpy(new_item->title, title);
        strcpy(new_item->sender, sender);
        strcpy(new_item->receiver, receiver);
        new_item->id = self->len + 1;
        self->list[self->len++] = new_item;
    } else {
        self->max_len *= 2; // 배열 크기 확장
        self->list = (DMMessage **)realloc(self->list, sizeof(DMMessage *) * self->max_len);
        dm_list_insert_message(self, title, content, sender, receiver); // 재귀 호출로 삽입 재시도
    }
}

//채팅 리스트에서 작성자 검색, 조건 만족하는 인덱스 리스트 반환
int* dm_list_search(DMList *self, char _id[], int type) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    for (int i = 0; i < self->len; i++) {
        bool flag = false;
        // first filter: addressed to me
        if (type == 1 && strcmp(self->list[i]->sender, _id) == 0) {
            flag = true;
        }
        else if (type == 2 && strcmp(self->list[i]->receiver, _id) == 0) {
            flag = true;
        }
        if (flag) ret[len++] = i;
    }
    ret[len] = -1; // 종료 표시
    return ret;
}

// 채팅 리스트에서 작성자, 수신자, id로 검색, 해당되는 인덱스 반환
int search_dm_by_elements(DMList *self, char sender[], char receiver[], int id) {
    int res = 0;
    printf("Searching DM from %s to %s with id %d\n", sender, receiver, id);
    for (int i = 0; i < self->len; i++) {
        printf("DM %d: from %s to %s with id %d\n", i, self->list[i]->sender, self->list[i]->receiver, self->list[i]->id);
        if(strcmp(self->list[i]->receiver,receiver) == 0) res++;
        if (strcmp(self->list[i]->sender, sender) == 0 &&
            strcmp(self->list[i]->receiver, receiver) == 0 &&
            self->list[i]->id == id) {
                printf("Found DM at index %d\n", i);
                return i;
        }
    }
    printf("res: %d\n", res);
    return -1; // not found
}

// 채팅 리스트 생성 함수
DMList* create_dm_list() {
    DMList* ret = (DMList *)malloc(sizeof(DMList));
    ret->insert_message = dm_list_insert_message;
    ret->search_message = dm_list_search;   
    ret->search_dm_by_elements = search_dm_by_elements;

    ret->max_len = 1;
    ret->list = (DMMessage **)malloc(sizeof (DMMessage *));
    ret->len = 0;
    return ret;
}

int dm_list_length(int *ind) {
    int ret = 0;
    while (ind[ret] != -1) ret++;
    return ret;
}

DMList *dm_messages = create_dm_list();