#include <iostream>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>

void get_dll_name(HANDLE process_handle) {
    DWORD bytes_used = 0;
    
    if (!EnumProcessModules(process_handle, nullptr, 0, &bytes_used)) {
        return ;
    }

    auto module_counts = bytes_used / sizeof(HMODULE);
    std::vector<HMODULE> module_handles(module_counts);

    if (EnumProcessModules(process_handle, module_handles.data(), static_cast<DWORD>(module_handles.size() * sizeof(HMODULE)), &bytes_used)) {
        for (const auto& module_handle : module_handles) {
            TCHAR module_name[MAX_PATH];
            if (GetModuleBaseName(process_handle, module_handle, module_name, std::size(module_name))) {
                std::wcout << L"    |__ Module: " << module_name << std::endl;
            }
        }
    }
}

void get_process_name(const std::vector<DWORD>& pids) {
    for (const auto& pid : pids) {
        if (pid == 0) continue;
        
        HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (process_handle != NULL) {
            TCHAR process_name[MAX_PATH] = TEXT("<unknown>");
            HMODULE module_handle;
            DWORD bytes_used;

            if (EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &bytes_used)) {
                GetModuleBaseName(process_handle, module_handle, process_name, std::size(process_name));
            }

            std::wcout << process_name << L" (PID:" << pid << L")" << std::endl;
            
            if (_tcsicmp(process_name, TEXT("explorer.exe")) == 0) {
                get_dll_name(process_handle);
            }
            
            CloseHandle(process_handle);
        }
    }
}

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
    get_process_name(pids);
    return 0;
}