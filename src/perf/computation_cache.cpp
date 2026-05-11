#include "computation_cache.h"

ComputationCache::ComputationCache() {}

bool ComputationCache::tryGet(const std::string& key, double& result) {
    auto it = cache.find(key);
    if (it != cache.end()) {
        result = it->second;
        return true;
    }
    return false;
}

void ComputationCache::store(const std::string& key, double result) {
    cache[key] = result;
}
