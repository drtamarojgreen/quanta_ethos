#pragma once

#include <string>
#include <chrono>

class RetentionManager {
public:
    RetentionManager(int days_to_keep);
    void applyPolicy(const std::string& directory);
    bool shouldDelete(const std::chrono::system_clock::time_point& creation_time) const;

private:
    int days;
};
