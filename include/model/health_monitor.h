#pragma once

#include <string>
#include <chrono>
#include <vector>

struct HealthMetrics {
    double latency_ms;
    double throughput;
    double perplexity;
    std::chrono::system_clock::time_point timestamp;
};

class HealthMonitor {
public:
    HealthMonitor();
    void recordMetrics(const HealthMetrics& metrics);
    std::vector<HealthMetrics> getHistory() const;
    bool isHealthy() const;

private:
    std::vector<HealthMetrics> history;
};
