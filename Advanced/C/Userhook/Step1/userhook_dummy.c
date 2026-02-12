#include <windows.h>
#include <stdio.h>
#include <share.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        FILE* f = _fsopen("C:\\tmp\\step1.txt", "a", _SH_DENYNO);
        if (f) {
            fprintf(f, "Step1 done.\n");
            fclose(f);
        }
    }
    return TRUE;
}