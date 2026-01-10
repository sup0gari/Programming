#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void system_exit() {
    printf("Invalid number.\n");
    exit(1);
} 

int main() {
    int loop;
    int mode;

    printf("Loop practice\n");
    printf("Select mode 1(for) / 2(while): ");
    // scanfは整数値を受け取った場合1を返すが、2Aなどと入力すると不具合が起こる。
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
                system_exit();
            }
        } else if (mode == 2) {
            printf("Mode \"while\" selected.\n");
            while (1) {
                printf("Looping... random number printed: %d\n", rand());
                printf("Exit 0 / Continue 1\n");
                int input;
                if (scanf("%d", &input) != 1) system_exit();
                if (input == 0) break;
                sleep(1);
            }
        } else {
            system_exit();
        }
    } else {
        system_exit();
    }
    return 0;
}