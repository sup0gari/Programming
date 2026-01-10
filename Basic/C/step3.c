#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int loop;
    int mode;

    printf("Loop practice\n");
    printf("Select mode 1(for) / 2(while): ");
    // scanfは整数値を受け取った場合1を返す。
    if (scanf("%d", &mode) == 1) {
        if (mode == 1) {
            printf("Mode \"for\" selected.\n");
            printf("How many times you wanna loop?: \n");
            if (scanf("%d", &loop) == 1) {
                for (int i = 0; i < loop; i++) {
                    printf("%d\n", i);
                    sleep(1);
                }
            } else {
                printf("Invalid number.\n");
                exit(1);
            }
        } else if (mode == 2) {
            printf("Mode \"while\" selected.\n");
            while (1) {
                printf("Looping... random number printed: %d\n", rand());
                printf("Exit 0 / Continue 1\n");
                int input;
                if (scanf("%d", &input) == 1) {
                    if (input == 0) {
                        break;
                    }
                } else {
                    printf("Invalid number.\n");
                    exit(1);
                }
                sleep(1);
            }
        } else {
            printf("Invalid number.\n");
            exit(1);
        }
    } else {
        printf("Invalid number.\n");
        exit(1);
    }
    return 0;
}