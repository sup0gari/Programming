#include <iostream>
#include <string>

int main() {
    int id;
    std::string name;
    
    std::cout << "Input number: ";
    std::cin >> id;

    std::cout << "Input name: ";
    std::cin >> name;

    std::cout << "Your name: " << name << std::endl;
    std::cout << "Your ID: " << id << std::endl;

    return 0;
}