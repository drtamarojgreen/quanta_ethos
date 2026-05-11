#pragma once

#include <atomic>
#include <mutex>

class ResourceGuard {
public:
    ResourceGuard(int max_concurrent);
    bool tryAcquire();
    void release();
    int getActiveCount() const;

private:
    int max_concurrent;
    std::atomic<int> active_requests;
};
