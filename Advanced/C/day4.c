#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE handle_ntdll = GetModuleHandleA("ntdll.dll");
    if (handle_ntdll == NULL) {
        printf("ntdll.dll not loaded.\n");
        return 1;
    }
    printf("ntdll.dll address: %p\n", handle_ntdll);
    
    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)handle_ntdll;
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
        return 1;
    }

    PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)((BYTE*)handle_ntdll + dos_header->e_lfanew);
    DWORD export_dir_rva = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY export_directory = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)handle_ntdll + export_dir_rva);

    DWORD* name_addresses = (DWORD*)((BYTE*)handle_ntdll + export_directory->AddressOfNames);
    WORD* ordinals = (WORD*)((BYTE*)handle_ntdll + export_directory->AddressOfNameOrdinals);
    DWORD* functions = (DWORD*)((BYTE*)handle_ntdll + export_directory->AddressOfFunctions);

    printf("--- EDR Hook Scanner Start ---\n");

    for (DWORD i = 0; i < export_directory->NumberOfNames; i++) {
        char* function_name = (char*)((BYTE*)handle_ntdll + name_addresses[i]);
        WORD ordinal = ordinals[i];
        void* function_address = (BYTE*)handle_ntdll + functions[ordinal];

        unsigned char first_byte = *(unsigned char*)function_address;
        if (first_byte == 0xE9) {
            printf("[!] HOOKED: %-30s | address: %p | bytes: %02X %02X %02X %02X %02X\n", 
                function_name, 
                function_address,
                ((unsigned char*)function_address)[0],
                ((unsigned char*)function_address)[1],
                ((unsigned char*)function_address)[2],
                ((unsigned char*)function_address)[3],
                ((unsigned char*)function_address)[4]
            );
        }
    }
}