#include "rate_limiter.h"

RateLimiter::RateLimiter(int max, std::chrono::seconds win) : max_requests(max), window(win) {}

bool RateLimiter::checkLimit(const std::string& user_id) {
    auto now = std::chrono::system_clock::now();
    auto& usage = usage_stats[user_id];

    if (now - usage.last_reset > window) {
        usage.count = 1;
        usage.last_reset = now;
        return true;
    }

    if (usage.count < max_requests) {
        usage.count++;
        return true;
    }

    return false;
}
