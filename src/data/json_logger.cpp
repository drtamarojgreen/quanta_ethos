#include "json_logger.h"
#include <iostream>
#include <sstream>

JsonLogger::JsonLogger() {}

std::string JsonLogger::formatJson(const std::string& level, const std::string& message, const std::map<std::string, std::string>& metadata) {
    std::ostringstream oss;
    oss << "{\"level\": \"" << level << "\", \"message\": \"" << message << "\"";
    for (const auto& pair : metadata) {
        oss << ", \"" << pair.first << "\": \"" << pair.second << "\"";
    }
    oss << "}";
    return oss.str();
}

void JsonLogger::log(const std::string& level, const std::string& message, const std::map<std::string, std::string>& metadata) {
    std::cout << formatJson(level, message, metadata) << std::endl;
}
