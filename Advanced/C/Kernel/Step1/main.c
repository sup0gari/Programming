#include <ntddk.h>

void Step1(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("Kernel Driver Step1: Goodbye Ring 0.\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("Kernel Driver Step1: Hello from Ring 0!\n");
    DriverObject->DriverUnload = Step1;

    return STATUS_SUCCESS;
}