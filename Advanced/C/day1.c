#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>

void get_dll_name(HANDLE process_handle) {
    HMODULE module_handles[1024];
    DWORD bytes_used;
    if (EnumProcessModules(process_handle, module_handles, sizeof(module_handles), &bytes_used)) {
        int module_count = bytes_used / sizeof(HMODULE);
        for (int i = 0; i < module_count; i++) {
            TCHAR module_name[MAX_PATH];
            if (GetModuleBaseName(process_handle, module_handles[i], module_name, sizeof(module_name) / sizeof(TCHAR))) {
                printf("    |__ Module: %s\n", module_name);
            }
        }
    }
}

void get_process_name(DWORD* process_ids, DWORD process_count) {
    for (DWORD i = 0; i < process_count; i++) {
        HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_ids[i]);
        if (process_handle != NULL) {
            HMODULE module_handle;
            DWORD bytes_used;
            TCHAR process_name[MAX_PATH] = TEXT("<unknown>");

            if (EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &bytes_used)) {
                GetModuleBaseName(process_handle, module_handle, process_name, sizeof(process_name) / sizeof(TCHAR));
            }

            printf("%-20s (PID: %u)\n", process_name, process_ids[i]);
            
            if (_tcsicmp(process_name, TEXT("explorer.exe")) == 0) {
                get_dll_name(process_handle);
            }
            CloseHandle(process_handle);
        }
    }
}

int main() {
    DWORD process_ids[1024];
    DWORD bytes_used;
    DWORD process_count;

    if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_used)) {
        return 1;
    }

    process_count = bytes_used / sizeof(DWORD);
    printf("%u processes found.\n", process_count);
    get_process_name(process_ids, process_count);
    return 0;
}