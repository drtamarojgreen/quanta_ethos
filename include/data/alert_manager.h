#pragma once

#include <string>

class AlertManager {
public:
    AlertManager(double error_threshold);
    void checkLog(const std::string& log_line);
    bool isAlerting() const;

private:
    double threshold;
    int error_count;
};
