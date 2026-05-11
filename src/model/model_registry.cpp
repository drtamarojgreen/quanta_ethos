#include "model_registry.h"
#include <algorithm>
#include <iostream>

ModelRegistry::ModelRegistry() {}

void ModelRegistry::loadModel(const std::string& model_id) {
    if (!isLoaded(model_id)) {
        std::cout << "Loading model: " << model_id << " into memory...\n";
        loaded_models.push_back(model_id);
    }
}

void ModelRegistry::unloadModel(const std::string& model_id) {
    auto it = std::find(loaded_models.begin(), loaded_models.end(), model_id);
    if (it != loaded_models.end()) {
        std::cout << "Unloading model: " << model_id << " from memory...\n";
        loaded_models.erase(it);
    }
}

bool ModelRegistry::isLoaded(const std::string& model_id) const {
    return std::find(loaded_models.begin(), loaded_models.end(), model_id) != loaded_models.end();
}

size_t ModelRegistry::getLoadedCount() const {
    return loaded_models.size();
}
