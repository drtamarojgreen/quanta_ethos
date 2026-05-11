#pragma once

#include <string>
#include <map>
#include <chrono>

struct Usage {
    int count;
    std::chrono::system_clock::time_point last_reset;
};

class RateLimiter {
public:
    RateLimiter(int max_requests, std::chrono::seconds window);
    bool checkLimit(const std::string& user_id);

private:
    int max_requests;
    std::chrono::seconds window;
    std::map<std::string, Usage> usage_stats;
};
