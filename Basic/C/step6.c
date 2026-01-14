#include <stdio.h>

void encrypt(char *str, int shift) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + shift;
            if (str[i] > 'Z') {
                str[i] -= 26;
            }
        }
    }
}

int main() {
    char message[100];
    int key;

    printf("Input uppercase letters: ");
    scanf("%99s", message);

    printf("Input 1 ~ 5: ");
    scanf("%d", &key);

    if (key < 1 || key > 5) {
        printf("Error: Input 1 ~ 5\n");
        return 1;
    }

    encrypt(message, key);
    printf("%d shifted : %s\n", key, message);

    return 0;
}