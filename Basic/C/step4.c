#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int nums[5] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        printf("nums[%d] = %d | Stack Address: %p\n", i, nums[i], &nums[i]);
    }

    // 2次元配列 3要素が2行分
    int nums2[2][3] = {
        {10, 20, 30},
        {40, 50, 60}
    };

    printf("\n\nRow | Col | Value | Address\n");
    printf("---------------------------\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" [%d] | [%d] |  %d  | %p\n", i, j, nums2[i][j], &nums2[i][j]);
        }
        printf("---------------------------\n");
    }

    // 3行10byte分（9byte + ヌルバイト）
    char users[3][10] = {"admin", "guest", "user"};
    char input[10];
    int found = 0;
    
    printf("\n\nEnter username: ");
    // 9byteのstringの入力制限
    scanf("%9s", input);

    for (int i = 0; i < 3; i++) {
        // strcmpは文字列一致で0を返す
        if (strcmp(users[i], input) == 0) {
            printf("Access granted: %s (User ID: %d)\n", users[i], i);
            found = 1;
            break;
        }
        sleep(2);
    }
    
    // 0じゃないとき
    if (!found) {
        printf("Access denied.\n");
    }
    return 0;
}