#include "allocation_tracker.h"

AllocationTracker::AllocationTracker() : current_usage(0) {}

void AllocationTracker::recordAllocation(size_t size) {
    current_usage += size;
}

void AllocationTracker::recordDeallocation(size_t size) {
    current_usage -= size;
}

size_t AllocationTracker::getCurrentUsage() const {
    return current_usage.load();
}
