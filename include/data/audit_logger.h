#pragma once

#include <string>
#include <vector>

struct AuditEntry {
    std::string timestamp;
    std::string action;
    std::string user;
    std::string details;
};

class AuditLogger {
public:
    AuditLogger(const std::string& log_path);
    void logEvent(const std::string& action, const std::string& user, const std::string& details);
    std::vector<AuditEntry> getEntries() const;

private:
    std::string path;
    std::vector<AuditEntry> entries;
};
