#include <windows.h>
#include <stdio.h>

void dump_syscall_number(const char* function_name) {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) return;

    void* address = GetProcAddress(ntdll, function_name);
    if (!address) {
        printf("%s not found.\n", function_name);
        return;
    }

    unsigned char* code = (unsigned char*)address;
    printf("[*] Function: %s at %p\n", function_name, address);
    printf ("   First 8 bytes: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", code[i]);
    }
    if (code[0] == 0x4C && code[1] == 0x8B && code[2] == 0xD1 && code[3] == 0xB8) {
        unsigned int ssn = *(unsigned int*)(code + 4);
        printf("\n    Detected System Call Number (SSN): 0x%04X\n", ssn);
    } else {
        printf("\n    [!] Function might be hooked by EDR!\n");
    }
    printf("------------------------------------------\n");
}

int main() {
    dump_syscall_number("NtAllocateVirtualMemory");
    dump_syscall_number("NtWriteVirtualMemory");
    dump_syscall_number("NtCreateThreadEx");
    return 0;
}