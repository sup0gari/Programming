#include <iostream>
#include <windows.h>
#include <stdexcept>
#include <string>

class SafeHandle {
private:
    HANDLE handle;
public:
    explicit SafeHandle(HANDLE handle) : handle(handle) {
        if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to acquire handle.");
        }
        std::cout << "[RAII] Handle acquired and protected." << std::endl;
    }

    ~SafeHandle() {
        if (handle != NULL || handle != INVALID_HANDLE_VALUE) {
            CloseHandle(handle);
            std::cout << "[RAII] Handle closed automatically by destructor." << std::endl;
        }
    }

    HANDLE get() const { return handle; }
    SafeHandle(const SafeHandle&) = delete;
    SafeHandle& operator=(const SafeHandle&) = delete;
};

void throw_exception(DWORD pid) {
    SafeHandle process_handle(OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid));
    
    bool memory_error = true;
    if (memory_error) {
        throw std::runtime_error("Failed.");
    }

    std::cout << "Program done safely." << std::endl;
}

int main() {
    try {
        DWORD target_pid = 10120;
        throw_exception(target_pid);
    } catch (const std::exception& e) {
        std::cerr << "[Catch] Error: " << e.what() << std::endl;
    }

    std::cout << "[Main] Program done safely." << std::endl;
    return 0;
}