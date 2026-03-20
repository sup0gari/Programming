#include <ntddk.h>

NTKERNELAPI UCHAR* PsGetProcessImageFileName(PEPROCESS Process);

void DriverUnload(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = DriverUnload;

    PEPROCESS StartProcess = PsGetCurrentProcess();
    PEPROCESS CurrentProcess = StartProcess;

    ULONG_PTR ActiveProcessLinksOffset = 0x1d8; // link offset
    do {
        HANDLE PID = PsGetProcessId(CurrentProcess);
        UCHAR* ImageName = PsGetProcessImageFileName(CurrentProcess);
        DbgPrint("PID: %p | Name: %s\n", PID, ImageName);

        PLIST_ENTRY NextLink = (PLIST_ENTRY)((PUCHAR)CurrentProcess + ActiveProcessLinksOffset);
        CurrentProcess =(PEPROCESS)((PUCHAR)NextLink->Flink - ActiveProcessLinksOffset);
    } while (CurrentProcess != StartProcess);

    return STATUS_SUCCESS;
}