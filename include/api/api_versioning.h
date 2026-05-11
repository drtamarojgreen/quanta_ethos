#pragma once

#include <string>
#include <map>

class ApiVersioning {
public:
    ApiVersioning();
    void registerRoute(const std::string& version, const std::string& path, const std::string& handler_id);
    std::string getHandler(const std::string& version, const std::string& path) const;

private:
    std::map<std::string, std::map<std::string, std::string>> routes;
};
