#include "retention_manager.h"
#include <iostream>

RetentionManager::RetentionManager(int d) : days(d) {}

bool RetentionManager::shouldDelete(const std::chrono::system_clock::time_point& creation) const {
    auto now = std::chrono::system_clock::now();
    auto age = std::chrono::duration_cast<std::chrono::hours>(now - creation).count();
    return age > (days * 24);
}

void RetentionManager::applyPolicy(const std::string& dir) {
    std::cout << "Cleaning up files in " << dir << " older than " << days << " days...\n";
}
