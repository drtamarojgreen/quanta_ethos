#include "alert_manager.h"
#include <iostream>

AlertManager::AlertManager(double t) : threshold(t), error_count(0) {}

void AlertManager::checkLog(const std::string& line) {
    if (line.find("ERROR") != std::string::npos) {
        error_count++;
        if (isAlerting()) {
            std::cout << "ALERT: Error count " << error_count << " exceeds threshold!\n";
        }
    }
}

bool AlertManager::isAlerting() const {
    return error_count > threshold;
}
