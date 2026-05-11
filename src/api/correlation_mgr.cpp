#include "correlation_mgr.h"

CorrelationMgr::CorrelationMgr() {}

std::string CorrelationMgr::generateId() {
    return "req-" + std::to_string(std::rand());
}

void CorrelationMgr::setCurrentId(const std::string& id) {
    current_id = id;
}

std::string CorrelationMgr::getCurrentId() const {
    return current_id;
}
