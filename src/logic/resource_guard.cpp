#include "resource_guard.h"

ResourceGuard::ResourceGuard(int max_concurrent) : max_concurrent(max_concurrent), active_requests(0) {}

bool ResourceGuard::tryAcquire() {
    int current = active_requests.load();
    while (current < max_concurrent) {
        if (active_requests.compare_exchange_weak(current, current + 1)) {
            return true;
        }
    }
    return false;
}

void ResourceGuard::release() {
    active_requests--;
}

int ResourceGuard::getActiveCount() const {
    return active_requests.load();
}
