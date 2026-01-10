#include <stdio.h>

int main() {
    int password;
    int action;

    printf("Input number(0~10): ");
    scanf("%d", &password);

    if (password >= 0 && password < 10) {
        if (password == 0 || password == 7 || password == 10) {
            printf("Authenticated.\n\n");

            printf("Choose action\n");
            printf("1: Display / 2: Print / 3: Exit\n");
            printf("Action: ");
            scanf("%d", &action);

            switch (action) {
                case 1:
                    printf("Display data.\n");
                    break;
                case 2:
                    printf("Print data.\n");
                    break;
                case 3:
                    printf("Exit.\n");
                    break;
                default:
                    printf("Invalid action.\n");
                    break;
            }
        } else if (password == 9) {
            printf("Authentication Error.\n");
        } else {
            printf("Authentication failed.\n");
        }
    } else {
        printf("Invalid password.\n");
    }

    return 0;
}