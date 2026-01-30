#include <windows.h>
#include <stdlib.h>

BOOL APIENTRY DllMain(HMODULE handle_module, DWORD ul_reason_for_call, LPVOID lp_reserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            system("whoami /priv > C:\\temp\\day6.txt & notepad C:\\temp\\day6.txt");
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}