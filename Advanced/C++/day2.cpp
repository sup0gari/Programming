#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <windows.h>

void dump_syscall_number(const std::string& function_name) {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) return;

    auto address = reinterpret_cast<unsigned char*>(GetProcAddress(ntdll, function_name.c_str()));
    if (!address) {
        std::cout << function_name << " not found." << std::endl;
        return;
    }

    for (int i = 0; i < 8; ++i) {
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(address[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    std::string input;

    std::cout << "Input Native API name." << std::endl;
    std::cin >> input;
    dump_syscall_number(input);
    return 0;
}