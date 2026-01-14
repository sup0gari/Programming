#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 16
#define DESC_SIZE 100

struct User {
    int id;
    char name[NAME_SIZE];
    char description[DESC_SIZE];
};

void input_user_name(struct User *u) {
    printf("\nInput your name (Max %zu letters)\n",(sizeof(u->name) - 1));
    // スペースを許容するためにscanfでなくfgetsを使用
    // fgets(アドレス, メモリ数, stdin) stdinは標準入力
    if (fgets(u->name, sizeof(u->name), stdin) != NULL) {
        // fgetsは\nも含むため、strcspnで改行箇所をヌルバイトに変更
        u->name[strcspn(u->name, "\n")] = '\0';
    }
}

void input_user_description(struct User *u) {
    printf("Input your description (Max %zu letters): \n", (sizeof(u->description) - 1));
    if (fgets(u->description, sizeof(u->description), stdin) != NULL) {
        u->description[strcspn(u->description, "\n")] = '\0';
    }
}

int main() {
    struct User user = {101, "", ""};
    
    input_user_name(&user);
    input_user_description(&user);

    printf("id                  : %d\n", user.id);
    printf("name                : %s\n", user.name);
    printf("description         : %s\n", user.description);
    printf("description address : %p\n", (void*)&user.description);

    return 0;
}