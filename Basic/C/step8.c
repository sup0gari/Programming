#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 値渡し
void pass_by_value(int num) {
    num += 900;
    printf("pass_by_value num : %d, address : %p\n", num, (void*)&num);
}

// 参照渡し（シングルポインタ）
void pass_by_ref(int *num) {
    *num *= 2;
    printf("pass_by_ref   num : %d, address  : %p\n", *num, (void*)num);
}

// ダブルポインタ
// double_pointer == &pointer, *double_pointer == pointer, **double_pointer == *pointer
// mainで宣言した*pointerはスタックで確保される。
// pass_by_double_pointerではpointerのアドレスをヒープに変更している。
// pointerの変更は*double_pointerの変更となる。
void pass_by_double_pointer(char **double_pointer) {
    *double_pointer = (char *)malloc(10);
    if (*double_pointer != NULL) {
        strcpy(*double_pointer, "HELLO");
    }
}

int main() {
    int x = 100;
    char *pointer = NULL;

    pass_by_value(x);
    pass_by_ref(&x);
    // pointer自体のアドレス
    pass_by_double_pointer(&pointer);
    if (pointer != NULL) {
        printf("double pointer    : %s, address: %p\n", pointer, (void *)pointer);
        free(pointer);
    }
    return 0;
}