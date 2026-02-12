#include <windows.h>
#include <stdio.h>
#include <share.h>

BYTE old_bytes[14];
void* target_function_address = NULL;

void write_log(const char* msg) {
    FILE* f = _fsopen("C:\\tmp\\step2.txt", "a", _SH_DENYNO);
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

void install_hook(const char* dll_name, const char* api_name) {
    target_function_address = (void*)GetProcAddress(GetModuleHandleA(dll_name), api_name);
    if (!target_function_address) {
        write_log("Target API not found.");
        return;
    }

    BYTE patch[14] = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00};
    DWORD old_protect;
    if (VirtualProtect(target_function_address, 14, PAGE_EXECUTE_READWRITE, &old_protect)) {
        memcpy(old_bytes, target_function_address, 14);
        write_log("Target API found and memory protection changed.");
        VirtualProtect(target_function_address, 14, old_protect, &old_protect);
    } else {
        write_log("VirtualProtect failed.");
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        write_log("Step2 done.");
        install_hook("ntdll.dll", "NtTerminateProcess");
    }
    return TRUE;
}