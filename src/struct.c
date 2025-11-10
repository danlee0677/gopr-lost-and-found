#include <string.h>
#include <stdbool.h>

#define MAX_TITLE_LEN 101
#define MAX_NAME_LEN 101
#define MAX_PICT_LEN 101
#define MAX_CONTENT_LEN 1001
#define ID_LEN 100
#define MAX_TAG_COUNT 10
#define MAX_MESSAGE_LEN 1001
#define MAX_PASSWORD_LEN 30

typedef enum {
    ELECTRONICS, // 전자기기
    CLOTHINGS, // 옷
    BAGS, // 가방
    STATIONERY, // 학용품
    PERSONAL, // 개인물품: 지갑, 신분증 등
    SPECIAL, // 기타 짬통
} TagEnum;

typedef struct {
    int _id; // 게시글별로 할당되는 고유 숫자
    char title[MAX_TITLE_LEN]; // 분실물 게시글 제목
    char content[MAX_CONTENT_LEN]; // 분실물 게시글 내용
    char picture[MAX_NAME_LEN]; // 사진
    bool deleted; // 삭제되었는지 여부
    int target_user; // 분실물 주인(없으면 -1로 뜸)
    TagEnum tag; // 태그
} LostItem;

typedef struct {
    int _id; // 유저별 할당 숫자
    char name[MAX_NAME_LEN]; // 유저명
    char password[MAX_PASSWORD_LEN]; // 비밀번호
    int student_id; // 학번
} User;

typedef struct LostItemList_t {
    void (*insert_lost_item)(struct LostItemList_t* self, char title[], char content[], char picture[], char userId[], TagEnum tag);
    void (*delete_lost_item)(struct LostItemList_t* self, int _id);
    int* (*search_tag)(struct LostItemList_t* self, TagEnum tag);
    int* (*search_content)(struct LostItemList_t* self, char **keywords, int keyword_count);
    int* (*search_user_id)(struct LostItemList_t *self, int _id);
    LostItem **list;
    int len, max_len;
} LostItemList;


/*
LostItemList 객체 할당 및 초기화
*/
void lost_item_list_insert_lost_item(LostItemList *self, char title[], char content[], char picture[], int target_user, TagEnum tag) {
    if (self->len < self->max_len) {
        LostItem *new_item = (LostItem *)malloc(sizeof(LostItem));
        strcpy(new_item->content, content);
        strcpy(new_item->title, title);
        strcpy(new_item->picture, picture);
        new_item->target_user = target_user;
        new_item->tag = tag;
        self->list[self->len++] = new_item;
    } else {
        self->max_len *= 2;
        self->list = (LostItem **)realloc(sizeof(LostItem *) * self->max_len);
        lost_item_list_insert_lost_item(self, title, content, picture, target_user, tag);
    }
}

int lost_item_list_delete_lost_item(LostItemList *self, int _id) {
    if (_id < self->max_len) {
        self->list[_id]->deleted = true;
    }
}

/*
return the index of matching items
array terminates with -1
always free after search
*/
int* lost_item_list_search_tag(LostItemList *self, TagEnum tag) {
    // always free the array that is returned
    // last item is -1
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    for (int i = 0; i < self->len; i++) {
        if (self->list[i]->tag == tag && !self->list[i]->deleted) {
            ret[len++] = i;
        }
    }
    ret[len] = -1;
    return ret;
}

/*
return the index of matching items
search in both content and title
array terminates with -1
always free after search
*/
int* lost_item_list_search_content(LostItemList *self, char **keywords, int keyword_count) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;
    for (int i = 0; i < self->len; i++) {
        if (self->list[i]->deleted || self->list[i]->target_user != -1) continue;
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

int *lost_item_list_search_user_id(LostItemList *self, int _id) {
    int *ret = (int *)malloc(sizeof(int) * (self->len + 1));
    int len = 0;

    for (int i = 0; i < self->len; i++) {
        if (!self->list[i]->deleted && self->list[i]->target_user == _id) ret[len++] = i;
    }
    ret[len] = -1;
    return ret;
}

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

typedef struct {
    char content[MAX_MESSAGE_LEN];
    char user[ID_LEN]; // user _id
} ChatMessage;

typedef struct Chat_t {
    ChatMessage **messages;
    int message_len;
    int max_message_len;
    void (*insert_message)(struct Chat_t* self, char content[], char user[]);
} ChatList;

void chat_insert_message(ChatList *self, char content[], char user[]) {
    if (self->message_len < self->max_message_len) {
        ChatMessage* new_message = (ChatMessage *)malloc(sizeof(ChatMessage));
        strcpy(new_message->content, content);
        strcpy(new_message->user, user);
        self->messages[self->message_len++] = new_message;
    } else {
        self->max_message_len *= 2;
        self->messages = (ChatMessage **)realloc(self->messages, sizeof(ChatMessage *) * self->max_message_len);
        chat_insert_message(self, content, user);
    }
}

ChatList* create_chat_list() {
    ChatList* ret = (ChatList *)malloc(sizeof(ChatList));
    ret->insert_message = chat_insert_message;
    ret->max_message_len = 1;
    ret->messages = (ChatMessage **)malloc(sizeof (ChatMessage *));
    ret->message_len = 0;
    return ret;
}

typedef struct UserList_t {
    User **list;
    int user_len, max_user_len;
    void (*create_user)(struct UserList_t* self, char password[], char name[], int student_id);
} UserList;

void user_list_create_user(UserList* self, char password[], char name[], int student_id) {
    if (self->user_len < self->max_user_len) {
        User* new_user = (User *)malloc(sizeof(User));
        strcpy(new_user->name, name);
        strcpy(new_user->password, password);
        new_user->student_id = student_id;
        new_user->_id = self->user_len;
        self->list[self->user_len++] = new_user;
    } else {
        self->max_user_len *= 2;
        self->list = (User **)realloc(self->list, sizeof(User *) * self->max_user_len);
        chat_insert_message(self, password, name, student_id);
    }
}

UserList* create_user_list() {
    UserList* ret = (UserList *)malloc(sizeof(UserList));
    ret->create_user = user_list_create_user;
    ret->user_len = 0;
    return ret;
}

UserList* temp;