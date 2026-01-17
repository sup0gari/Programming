#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>

DWORD find_and_alloc(TCHAR* target_process) {
    DWORD process_ids[1024], bytes_used, process_count;
    DWORD found = 0;
    
    if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_used)) {
        printf("EnumProcesses failed.\n");
        return 0;
    }

    process_count = bytes_used / sizeof(DWORD);
    for (DWORD i = 0; i < process_count; i++) {
        
        HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_ids[i]);
        if (handle != NULL) {
            TCHAR process_name[MAX_PATH] = TEXT("<unknown>");
            HMODULE handle_module;
            DWORD cb_needed;
            if (EnumProcessModules(handle, &handle_module, sizeof(handle_module), &cb_needed)) {
                GetModuleBaseName(handle, handle_module, process_name, sizeof(process_name) / sizeof(TCHAR));
            }

            if (_tcsicmp(process_name, target_process) == 0) {
                found = 1;
                printf("[*] Found process: %s (PID: %u)\n", process_name, process_ids[i]);
                HANDLE target_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_ids[i]);

                if (target_handle == NULL) {
                    printf("[!] Failed to get FULL ACCESS handle for %s. Error: %lu\n", process_name, GetLastError());
                    CloseHandle(handle);
                    return 1;
                }

                SIZE_T size = 1024;
                LPVOID allocated_memory = VirtualAllocEx(target_handle, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

                if (allocated_memory != NULL) {
                    printf("    |__ Successfully allocated at: %p\n", allocated_memory);
                    printf("    |__ Press Enter to free and continue searching...\n");

                    getchar(); 

                    VirtualFreeEx(target_handle, allocated_memory, 0, MEM_RELEASE);
                    printf("    |__ Memory freed.\n");
                } else {
                    printf("    |__ VirtualAllocEx failed. Error: %lu\n", GetLastError());
                }
            }
            CloseHandle(handle);
        }
    }
    return found;
}

int main() {
    TCHAR input_process_name[MAX_PATH];
    DWORD result;

    printf("Enter process name to find (e.g. notepad.exe, explorer.exe): ");
    _fgetts(input_process_name, MAX_PATH, stdin);
    
    input_process_name[_tcscspn(input_process_name, TEXT("\n"))] = 0;
    result = find_and_alloc(input_process_name);
    if (!result) {
        printf("Not found: %s", input_process_name);
    }
    return 0;
}