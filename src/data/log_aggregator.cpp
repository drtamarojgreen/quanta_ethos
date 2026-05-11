#include "log_aggregator.h"
#include <iostream>

LogAggregator::LogAggregator(const std::string& endpoint) : endpoint(endpoint) {}

void LogAggregator::ship(const std::string& log) {
    buffer.push_back(log);
    if (buffer.size() > 10) flush();
}

bool LogAggregator::flush() {
    if (buffer.empty()) return true;
    std::cout << "Shipping " << buffer.size() << " logs to " << endpoint << "...\n";
    buffer.clear();
    return true;
}
