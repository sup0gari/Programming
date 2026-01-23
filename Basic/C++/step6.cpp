#include <iostream>
#include <vector>
#include <memory>

class Base {
public:
    virtual void Speak() {
        std::cout << "Someone speaking." << std::endl;
    }

    virtual ~Base(){}
};

class Japanese : public Base {
public:
    void Speak() override {
        std::cout << "Konnitiwa." << std::endl;
    }
};

class English : public Base {
public:
    void Speak() override {
        std::cout << "Hello." << std::endl;
    }
};

int main() {
    std::vector<std::unique_ptr<Base>> people;

    people.push_back(std::make_unique<Japanese>());
    people.push_back(std::make_unique<English>());

    for (const auto& person : people) {
        person->Speak();
    }
    return 0;
}