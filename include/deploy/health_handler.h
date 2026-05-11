#pragma once

#include <string>

class HealthHandler {
public:
    HealthHandler();
    std::string getHealthStatus() const;
    bool isLive() const;
};
