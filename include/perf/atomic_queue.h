#pragma once

#include <atomic>
#include <vector>

template<typename T>
class AtomicQueue {
public:
    AtomicQueue(size_t size) : buffer(size), head(0), tail(0) {}
    bool push(const T& item);
    bool pop(T& item);

private:
    std::vector<T> buffer;
    std::atomic<size_t> head, tail;
};

template<typename T>
bool AtomicQueue<T>::push(const T& item) {
    size_t t = tail.load(std::memory_order_relaxed);
    buffer[t % buffer.size()] = item;
    tail.store(t + 1, std::memory_order_release);
    return true;
}

template<typename T>
bool AtomicQueue<T>::pop(T& item) {
    size_t h = head.load(std::memory_order_relaxed);
    if (h == tail.load(std::memory_order_acquire)) return false;
    item = buffer[h % buffer.size()];
    head.store(h + 1, std::memory_order_release);
    return true;
}
