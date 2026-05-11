#pragma once

#include <string>
#include <vector>

class LogAggregator {
public:
    LogAggregator(const std::string& endpoint);
    void ship(const std::string& structured_log);
    bool flush();

private:
    std::string endpoint;
    std::vector<std::string> buffer;
};
