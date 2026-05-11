#include "health_monitor.h"

HealthMonitor::HealthMonitor() {}

void HealthMonitor::recordMetrics(const HealthMetrics& metrics) {
    history.push_back(metrics);
}

std::vector<HealthMetrics> HealthMonitor::getHistory() const {
    return history;
}

bool HealthMonitor::isHealthy() const {
    if (history.empty()) return true;
    // Simple health check: last latency < 2000ms
    return history.back().latency_ms < 2000.0;
}
