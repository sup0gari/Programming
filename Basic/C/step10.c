#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define NAME_SIZE 16
#define PASSWORD_SIZE 16

struct User {
    int id;
    char name[NAME_SIZE];
    char password[PASSWORD_SIZE];
    void (*login)(struct User *);
};

void login_failed(struct User *u) {
    printf("Login failed as %s.\n", u->name);
    exit(1);
}

void authenticate(struct User *u) {
    char input_password[PASSWORD_SIZE];
    printf("Input password for %s: ", u->name);

    if (fgets(input_password, sizeof(input_password), stdin) != NULL) {
        input_password[strcspn(input_password, "\n")] = '\0';

        if (strcmp(input_password, u->password) == 0) {
            printf("Login successful as %s.\n", u->name);
        } else {
            login_failed(u);
        }
    }
}

int main() {
    struct User user = {
        .id = 100
    };

    char input_name[NAME_SIZE];
    printf("Input username: ");
    if (fgets(input_name, sizeof(input_name), stdin) != NULL) {
        input_name[strcspn(input_name, "\n")] = '\0';
    }

    if (strcmp(input_name, "root") == 0) {
        strncpy(user.name, "root", NAME_SIZE - 1);
        strncpy(user.password, "root1234", PASSWORD_SIZE - 1);
        user.login = authenticate;
    } else if (strcmp(input_name, "user") == 0) {
        strncpy(user.name, "user", NAME_SIZE - 1);
        strncpy(user.password, "user7890", PASSWORD_SIZE - 1);
        user.login = authenticate;
    } else {
        strncpy(user.name, input_name, NAME_SIZE - 1);
        user.login = login_failed;
    }

    user.login(&user);
    return 0;
}