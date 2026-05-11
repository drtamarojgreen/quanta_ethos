#pragma once

#include <string>
#include <map>

class SecretsVault {
public:
    SecretsVault();
    void storeSecret(const std::string& key, const std::string& value);
    std::string getSecret(const std::string& key) const;

private:
    std::map<std::string, std::string> vault;
};
