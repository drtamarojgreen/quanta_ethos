#pragma once

#include <string>

class CorrelationMgr {
public:
    CorrelationMgr();
    std::string generateId();
    void setCurrentId(const std::string& id);
    std::string getCurrentId() const;

private:
    std::string current_id;
};
