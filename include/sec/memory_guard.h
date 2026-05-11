#pragma once

#include <memory>

class MemoryGuard {
public:
    MemoryGuard();
    void* allocate(size_t size);
    void deallocate(void* ptr);
};
