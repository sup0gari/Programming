#include <iostream>
#include <string>

class User {
private:
    std::string name;
    int id;

public:
    User(std::string input_name = "guest", int input_id = 1) {
        name = input_name;
        id = input_id;
        std::cout << "Constructor: " << name << " is created." << std::endl;
    }

    ~User() {
        std::cout << "Destructor: " << name << " is destroyed." << std::endl;
    }

    int get_id() {
        return id;
    }

    std::string get_name() {
        return name;
    }

    void show() {
        std::cout << "Name: " << name << ", Id: " << id << std::endl;
    }
};

void check_user(User& user) {
    if (user.get_name() != "guest") {
        std::cout << "You are not default user." << std::endl;
    } else {
        std::cout << "You are guest." << std::endl;
    }
}

int main() {
    User user1;
    std::string input_name;
    int input_id;

    std::cout << "Input your name: ";
    std::getline(std::cin, input_name);
    std::cout << "Input your id: ";
    std::cin >> input_id;
    User user2(input_name, input_id);

    user2.show();
    if (user2.get_id() > 10) {
        std::cout << "High privilege user ID: " << user2.get_id() << std::endl;
    }

    check_user(user2);

    return 0;
}