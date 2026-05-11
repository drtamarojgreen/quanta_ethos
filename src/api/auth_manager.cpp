#include "auth_manager.h"

AuthManager::AuthManager() {}

void AuthManager::addApiKey(const std::string& key, const std::string& role) {
    api_keys[key] = role;
}

bool AuthManager::authenticate(const std::string& key) {
    return api_keys.find(key) != api_keys.end();
}

bool AuthManager::isAuthorized(const std::string& key, const std::string& required_role) {
    auto it = api_keys.find(key);
    if (it == api_keys.end()) return false;

    if (required_role == "user") return true;
    if (required_role == "admin") return it->second == "admin";

    return false;
}
