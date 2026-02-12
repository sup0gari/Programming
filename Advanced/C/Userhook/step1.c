#include <windows.h>
#include <stdio.h>

int main() {
    DWORD pid;
    char dll_path[] = "C:\\tmp\\userhook.dll";

    printf("Input target PID: ");
    scanf_s("%d", &pid);

    HANDLE target_process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!target_process_handle) {
        printf("OpenProcess failed. Error code: %d\n", GetLastError());
        return 1;
    }

    LPVOID target_process_buffer = VirtualAllocEx(target_process_handle, NULL, sizeof(dll_path), MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(target_process_handle, target_process_buffer, (LPVOID)dll_path, sizeof(dll_path), NULL);
    PTHREAD_START_ROUTINE load_library_address = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    HANDLE target_process_thread = CreateRemoteThread(target_process_handle, NULL, 0, load_library_address, target_process_buffer, 0, NULL);
    if (target_process_thread) {
        printf("userhook.dll injected successfully.\n");
        WaitForSingleObject(target_process_thread, INFINITE);
        CloseHandle(target_process_thread);
    } else {
        printf("Injection failed.\n");
    }

    VirtualFreeEx(target_process_handle, target_process_buffer, 0, MEM_RELEASE);
    CloseHandle(target_process_handle);
    return 0;
}