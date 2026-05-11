#include "model_cache.h"

ModelCache::ModelCache(size_t max_size) : max_size(max_size) {}

bool ModelCache::tryGet(const std::string& prompt, std::string& output) {
    auto it = cache.find(prompt);
    if (it != cache.end()) {
        output = it->second;
        return true;
    }
    return false;
}

void ModelCache::store(const std::string& prompt, const std::string& output) {
    if (cache.size() >= max_size) {
        cache.erase(cache.begin()); // Simple FIFO eviction
    }
    cache[prompt] = output;
}

void ModelCache::clear() {
    cache.clear();
}
