#pragma once

#include <string>
#include <unordered_map>

class ComputationCache {
public:
    ComputationCache();
    bool tryGet(const std::string& key, double& result);
    void store(const std::string& key, double result);

private:
    std::unordered_map<std::string, double> cache;
};
