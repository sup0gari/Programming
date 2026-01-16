#include <iostream>
#include <string>
#include <vector>

int main() {
    int ports[] = {22, 80, 443, 3389};
    int mode;

    std::cout << "1: Display ports" << std::endl;
    std::cout << "2: Display status" << std::endl;
    std::cout << "3: Select mode" << std::endl;
    std::cout << "Input number: ";

    if (!(std::cin >> mode)) {
        std::cout << "Error. It's not the number.";
        return 1;
    }

    switch(mode) {
        case 1:
            std::cout << "Display ports..." << std::endl;
            for (int port : ports) {
                std::cout << "Port: " << port << std::endl;
            }
            break;
        case 2:
            int check_port;
            std::cout << "Select the port number (22, 80, 443, 3389)" << std::endl;
            std::cin >> check_port;

            if (bool found = false; check_port > 0) {
                for (int port : ports) {
                    if (port == check_port) found = true;
                }
                
                if (found) {
                    std::cout << "Port: " << check_port << " is monitored." << std::endl;
                } else {
                    std::cout << "Not found: " << check_port << std::endl;
                }
            }
            break;
        case 3:
            std::cout << "1: Fast, 2: Full" << std::endl;
            int mode;
            std::cin >> mode;
            if (mode == 2) {
                std::cout << "Full scan done." << std::endl;
            } else {
                std::cout << "Fast scan done." << std::endl;
            }
            break;
        default:
            std::cout << "Invalid mode. Exiting..." << std::endl;
            break;
    }
    return 0;
}