#include "audit_logger.h"
#include <chrono>
#include <ctime>

AuditLogger::AuditLogger(const std::string& p) : path(p) {}

void AuditLogger::logEvent(const std::string& action, const std::string& user, const std::string& details) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string ts = std::ctime(&now);
    if (!ts.empty()) ts.pop_back(); // Remove newline
    entries.push_back({ts, action, user, details});
}

std::vector<AuditEntry> AuditLogger::getEntries() const {
    return entries;
}
