#pragma once

#include <string>
#include <map>
#include <vector>

class ModelRegistry {
public:
    ModelRegistry();
    void loadModel(const std::string& model_id);
    void unloadModel(const std::string& model_id);
    bool isLoaded(const std::string& model_id) const;
    size_t getLoadedCount() const;

private:
    std::vector<std::string> loaded_models;
};
