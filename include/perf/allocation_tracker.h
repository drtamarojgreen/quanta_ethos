#pragma once

#include <cstddef>
#include <atomic>

class AllocationTracker {
public:
    AllocationTracker();
    void recordAllocation(size_t size);
    void recordDeallocation(size_t size);
    size_t getCurrentUsage() const;

private:
    std::atomic<size_t> current_usage;
};
