#pragma once

#include <string>
#include <vector>
#include <queue>
#include <mutex>

struct Request {
    std::string prompt;
    int priority;
};

class BatchScheduler {
public:
    BatchScheduler();
    void addRequest(const Request& req);
    std::vector<Request> getNextBatch(size_t max_size);

private:
    std::priority_queue<Request, std::vector<Request>, auto(*)(const Request&, const Request&)->bool> queue;
    std::mutex mtx;
};
