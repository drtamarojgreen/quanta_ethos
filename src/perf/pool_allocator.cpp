#include "pool_allocator.h"
#include <cstdlib>

PoolAllocator::PoolAllocator(size_t bs, size_t n) : block_size(bs) {
    for (size_t i = 0; i < n; ++i) blocks.push_back(std::malloc(bs));
}

PoolAllocator::~PoolAllocator() {
    for (void* b : blocks) std::free(b);
}

void* PoolAllocator::allocate() {
    if (blocks.empty()) return nullptr;
    void* b = blocks.back();
    blocks.pop_back();
    return b;
}

void PoolAllocator::deallocate(void* ptr) {
    blocks.push_back(ptr);
}
