#include "health_handler.h"

HealthHandler::HealthHandler() {}

std::string HealthHandler::getHealthStatus() const {
    return "{\"status\": \"UP\"}";
}

bool HealthHandler::isLive() const {
    return true;
}
