#include "api_versioning.h"

ApiVersioning::ApiVersioning() {}

void ApiVersioning::registerRoute(const std::string& version, const std::string& path, const std::string& handler) {
    routes[version][path] = handler;
}

std::string ApiVersioning::getHandler(const std::string& version, const std::string& path) const {
    auto v_it = routes.find(version);
    if (v_it != routes.end()) {
        auto p_it = v_it->second.find(path);
        if (p_it != v_it->second.end()) {
            return p_it->second;
        }
    }
    return "";
}
