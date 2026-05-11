#pragma once

#include <string>
#include <map>

class JsonLogger {
public:
    JsonLogger();
    void log(const std::string& level, const std::string& message, const std::map<std::string, std::string>& metadata = {});
    std::string formatJson(const std::string& level, const std::string& message, const std::map<std::string, std::string>& metadata);
};
