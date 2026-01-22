#include <iostream>
#include <memory>

class Target {
public:
    std::string name;
    Target(std::string n) : name(n) {std::cout << "Allocated: " << name << std::endl;}
    ~Target() {std::cout << "Freed: " << name << std::endl;}
    void hello() {
        std::cout << name << " is executed." << std::endl;
    }
};

void raw_pointer() {
    std::cout << "\n--- 1. Raw Pointer ---" << std::endl;
    Target* t = new Target("Manual_Target");
    t->hello();

    delete t;
    std::cout << "Deleted (Raw Pointer)" << std::endl;
}

void unique_pointer() {
    std::cout << "\n--- 2. Unique Pointer ---" << std::endl;
    {
        std::unique_ptr<Target> u_ptr = std::make_unique<Target>("Unique_Target");
        u_ptr->hello();
    }
    std::cout << "Out of scope (Unique Pointer)" << std::endl;
}

void shared_pointer() {
    std::cout << "\n--- 3. Shared Pointer ---" << std::endl;
    std::shared_ptr<Target> s_ptr_1 = std::make_shared<Target>("Shared_Target");
    {
        std::shared_ptr<Target> s_ptr_2 = s_ptr_1;
        std::cout << "Refered: " << s_ptr_1.use_count() << std::endl;
        s_ptr_2->hello();
    }
    std::cout << "Refered: " << s_ptr_1.use_count() << std::endl;
}

int main() {
    raw_pointer();
    unique_pointer();
    shared_pointer();

    std::cout << "Exiting." << std::endl;
    return 0;
}