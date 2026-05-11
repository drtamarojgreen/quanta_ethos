#pragma once

#include <string>
#include <map>

class AuthManager {
public:
    AuthManager();
    void addApiKey(const std::string& key, const std::string& user_role);
    bool authenticate(const std::string& key);
    bool isAuthorized(const std::string& key, const std::string& required_role);

private:
    std::map<std::string, std::string> api_keys;
};
