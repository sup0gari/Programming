#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void func_iterator(std::vector<std::string>& data) {
    std::cout << "Iterator" << std::endl;

    auto it = data.begin();
    if (it != data.end()) {
        std::cout << "First element: " << *it << std::endl;
        it++;
        if (it != data.end()) {
            std::cout << "Second element: " << *it << std::endl;
        }
    }

    std::cout << "Reverse output: ";
    for (auto rit = data.rbegin(); rit != data.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n" << std::endl;
}

void func_sort(std::vector<std::string>& data) {
    std::cout << "Sort" << std::endl;

    std::sort(data.begin(), data.end());
    std::cout << "Sorted data: ";
    for (const auto& s : data) std::cout << s << " ";
    std::cout << "\n" << std::endl;
}

void func_find(std::vector<std::string>& data) {
    std::cout << "Find" << std::endl;
    std::string target;
    std::cout << "Enter keyword to search: ";
    std::cin >> target;

    auto it = std::find(data.begin(), data.end(), target);
    if (it != data.end()) {
        auto index = std::distance(data.begin(), it);
        std::cout << "[Found] '" << target << "' at index " << index << std::endl;
    } else {
        std::cout << "[Not Found] '" << target << "' is not in the list." << std::endl;
    }
}

int main() {
    std::vector<std::string> logs = {"apple", "banana", "cherry", "dragonfruit", "elderberries"};

    func_iterator(logs);
    func_sort(logs);
    func_find(logs);

    return 0;
}