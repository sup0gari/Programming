#include <windows.h>
#include <stdio.h>
#include <share.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        FILE* f = _fsopen("C:\\tmp\\step1_success.txt", "a", _SH_DENYNO);
        if (f) {
            fprintf(f, "Step1 Done.\n");
            fclose(f);
        }
    }
    return TRUE;
}