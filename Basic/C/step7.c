#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int size;
    char *str;
    char stack_str[100];

    printf("How many letters you wanna input?: ");
    scanf("%d", &size);

    // 入力サイズ + ヌルバイト分のメモリを確保
    str = (char *)malloc(sizeof(char) * (size + 1));

    // 確保できなかったとき
    if (str == NULL) {
        printf("No space for heap.\n");
        return 1;
    }

    printf("You can input %d letters: \n", size);
    scanf("%s", str);
    
    // strncpy(コピー先, コピー元のアドレス, メモリ数)
    strncpy(stack_str, str, sizeof(stack_str) - 1);
    // 末尾をヌルバイト書き込み
    stack_str[sizeof(stack_str) - 1] = '\0';
    
    printf("Stack string: %s | Address: %p\n", stack_str, (void *)&stack_str);
    printf("Letters:      %s | Address: %p\n", str, (void *)str);

    free(str);
    return 0;
}