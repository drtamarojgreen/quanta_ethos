#pragma once

#include <vector>
#include <cstddef>

class PoolAllocator {
public:
    PoolAllocator(size_t block_size, size_t num_blocks);
    ~PoolAllocator();
    void* allocate();
    void deallocate(void* ptr);

private:
    size_t block_size;
    std::vector<void*> blocks;
};
