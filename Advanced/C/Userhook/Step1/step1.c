#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <PID> <DLL_PATH>\n", argv[0]);
        return 1;
    }

    DWORD pid = (DWORD)atoi(argv[1]);
    char* dll_path = argv[2];
    HANDLE target_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!target_handle) {
        printf("OpenProcess failed.\n");
        return 1;
    }

    SIZE_T path_len = strlen(dll_path) + 1;
    LPVOID target_buffer = VirtualAllocEx(target_handle, NULL, path_len, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(target_handle, target_buffer, (LPVOID)dll_path, path_len, NULL);
    PTHREAD_START_ROUTINE remote_process = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    HANDLE remote_thread = CreateRemoteThread(target_handle, NULL, 0, remote_process, target_buffer, 0, NULL);

    if (remote_thread) {
        printf("Injected successfully.\n");
        WaitForSingleObject(remote_thread, INFINITE);
        CloseHandle(remote_thread);
    } else {
        printf("CreateRemoteThread failed.\n");
    }

    VirtualFreeEx(target_handle, target_buffer, 0, MEM_RELEASE);
    CloseHandle(target_handle);
    return 0;
}