#include <iostream>
#include <windows.h>
#include <string>

class LoopManager {
    public:
        
    void Run(int count) {
        std::cout << "[for] Looping " << count << " times." << std::endl;
        for (int i = 0; i < count; i++) {
            std::cout << i << std::endl;
            Sleep(1000);
        }
    }

    void Run(const std::string& msg) {
        std::cout << "[while] " << msg << std::endl;
        while (true) {
            std::cout << "Looping... Random: " << rand() << std::endl;
            std::cout << "Exit 0 / Continue any numbers(exclude 0): ";
            int input;
            if (!(std::cin >> input) || input == 0) break;
            Sleep(1000);
        }
    }

    void Exit(const std::string& error_msg = "Invalid number.") {
        std::cerr << "[!] " << error_msg << std::endl;
        exit(1);
    }
};

int main() {
    LoopManager loop_manager;
    int mode;

    std::cout << "Select mode 1 (for) / 2 (while): ";
    if (!(std::cin >> mode)) {
        loop_manager.Exit();
    }

    if (mode == 1) {
        int count;
        std::cout << "How many times?: ";
        if (std::cin >> count) {
            loop_manager.Run(count);
        } else {
            loop_manager.Exit();
        }
    } else if (mode == 2) {
        std::string msg = "Stating while loop...";
        loop_manager.Run(msg);
    } else {
        loop_manager.Exit("Input 1 or 2.");
    }

    return 0;
}