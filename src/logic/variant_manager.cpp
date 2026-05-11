#include "variant_manager.h"
#include <iostream>

VariantManager::VariantManager() {
    std::random_device rd;
    gen.seed(rd());
}

void VariantManager::addExperiment(const std::string& name, const std::map<std::string, float>& variants) {
    experiments[name] = variants;
}

std::string VariantManager::selectVariant(const std::string& experiment_name) {
    auto it = experiments.find(experiment_name);
    if (it == experiments.end()) return "default";

    std::uniform_real_distribution<> dis(0.0, 1.0);
    float pick = dis(gen);
    float cumulative = 0.0f;

    for (const auto& variant : it->second) {
        cumulative += variant.second;
        if (pick <= cumulative) return variant.first;
    }

    return it->second.begin()->first;
}
