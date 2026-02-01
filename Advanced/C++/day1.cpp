#include <iostream>
#include <vector>
#include <windows.h>
#include <psapi.h>

void print_process_info(const std::vector<DWORD> pids);

int main() {
    std::vector<DWORD> pids(1024);
    DWORD bytes_used = 0;

    if (!EnumProcesses(pids.data(), static_cast<DWORD>(pids.size() * sizeof(DWORD)), &bytes_used)) {
        std::cerr << "EnumProcesses failed." << std::endl;
        return 1;
    }
    
    auto process_count = bytes_used / sizeof(DWORD);
    pids.resize(process_count);
    std::cout << process_count << " processes found." << std::endl;
    // print_process_info(pids);
    return 0;
}