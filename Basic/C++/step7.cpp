#include <iostream>
#include <vector>
#include <string>
#include <map>

void display_map(std::map<std::string, int>& map) {
    std::string key;
    std::cout << "Input word (Exit to 'end'): " << std::endl;

    while (std::cin >> key && key != "end") {
        map[key]++;
    }

    std::cout << "\n--- Result ---" << std::endl;
    for (const auto& pair : map) {
        std::cout << "Word: " << pair.first << " | Count: " << pair.second << std::endl;
    }
}

void display_string(std::string& string) {
    std::cout << "Input your name: ";
    std::cin >> string;

    std::string tagged = string + "_analyzed";
    std::cout << "Tagged String: " << tagged << std::endl;

    if (string == "admin") {
        std::cout << "[Alert] admin found." << std::endl;
    } else if (string != "guest") {
        std::cout << "[Info] you're not guest." << std::endl;
    }

    std::string target = "bad";
    std::string replacement = "good";

    size_t pos = string.find(target);
    if (pos != std::string::npos) {
        std::cout << "[Detect] 'bad' keyword found at index: " << pos << std::endl;
        string.replace(pos, target.length(), replacement);
        std::cout << "Replaced String: " << string << std::endl;
    }

    if (string.length() >= 5) {
        std::string prefix = string.substr(0, 5);
        std::cout << "Prefix (first 5 chars): " << prefix << std::endl;
    }
}

void display_vector(std::vector<std::string>& vector_list) {
    std::string input;
    std::cout << "Input string (Exit to 'end'): " << std::endl;

    while (std::cin >> input && input != "end") {
        vector_list.push_back(input);
    }

    if (!vector_list.empty()) {
        vector_list.insert(vector_list.begin(), "START");
    }

    if (vector_list.size() >= 2) {
        vector_list.erase(vector_list.begin() + 1);
    }

    std::cout << "\n--- Display data ---" << std::endl;
    for (const std::string& str : vector_list) {
        std::cout << "Data: " << str << std::endl;
    }
}

int main() {
    std::vector<std::string> vector_list;
    std::string string;
    std::map<std::string, int> map;

    display_vector(vector_list);
    display_string(string);
    display_map(map);
    return 0;
}