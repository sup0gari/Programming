#include <windows.h>
#include <stdio.h>
#include <share.h>

typedef NTSTATUS (NTAPI *P_NtTerminateProcess)(HANDLE, NTSTATUS);
P_NtTerminateProcess original_NtTerminateProcess = NULL;

NTSTATUS NTAPI hook_NtTerminateProcess(HANDLE handle, NTSTATUS exit_status) {
    FILE* f = _fsopen("C:\\tmp\\hook.txt", "a", _SH_DENYNO);
    if (f) {
        fprintf(f, "[HOOK] NtTerminateProcess called. Handle: %p, Status: %d\n", handle, exit_status);
        fclose(f);
    }

    return original_NtTerminateProcess(handle, exit_status);
}

void install_hook() {
    void* target_address = (void*)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtTerminateProcess");
    if (!target_address) return;

    original_NtTerminateProcess = (P_NtTerminateProcess)VirtualAlloc(NULL, 30, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!original_NtTerminateProcess) return;

    memcpy(original_NtTerminateProcess, target_address, 14);

    BYTE jmp[14] = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00};
    void* return_address = (char*)target_address + 14;
    memcpy(&jmp[6], &return_address, 8);
    memcpy((char*)original_NtTerminateProcess + 14, jmp, 14);

    BYTE patch[14] = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00};
    void* userhook_address = (void*)hook_NtTerminateProcess;
    memcpy(&patch[6], &userhook_address, 8);

    DWORD old_protect;
    if (VirtualProtect(target_address, 14, PAGE_EXECUTE_READWRITE, &old_protect)) {
        memcpy(target_address, patch, 14);
        VirtualProtect(target_address, 14, old_protect, &old_protect);
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID lp_reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        install_hook();
    }
    return TRUE;
}