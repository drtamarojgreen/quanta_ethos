#pragma once

#include <string>
#include <map>

class ModelCache {
public:
    ModelCache(size_t max_size);
    bool tryGet(const std::string& prompt, std::string& output);
    void store(const std::string& prompt, const std::string& output);
    void clear();

private:
    size_t max_size;
    std::map<std::string, std::string> cache;
};
