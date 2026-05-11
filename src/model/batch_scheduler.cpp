#include "batch_scheduler.h"

bool compareRequests(const Request& a, const Request& b) {
    return a.priority < b.priority;
}

BatchScheduler::BatchScheduler() : queue(compareRequests) {}

void BatchScheduler::addRequest(const Request& req) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(req);
}

std::vector<Request> BatchScheduler::getNextBatch(size_t max_size) {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<Request> batch;
    while (!queue.empty() && batch.size() < max_size) {
        batch.push_back(queue.top());
        queue.pop();
    }
    return batch;
}
