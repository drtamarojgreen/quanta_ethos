#include "memory_guard.h"
#include <cstdlib>

MemoryGuard::MemoryGuard() {}

void* MemoryGuard::allocate(size_t size) {
    return std::malloc(size);
}

void MemoryGuard::deallocate(void* ptr) {
    std::free(ptr);
}
