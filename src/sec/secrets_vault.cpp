#include "secrets_vault.h"

SecretsVault::SecretsVault() {}

void SecretsVault::storeSecret(const std::string& k, const std::string& v) {
    vault[k] = v;
}

std::string SecretsVault::getSecret(const std::string& k) const {
    auto it = vault.find(k);
    return it != vault.end() ? it->second : "";
}
