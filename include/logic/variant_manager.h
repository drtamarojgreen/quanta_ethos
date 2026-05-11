#pragma once

#include <string>
#include <map>
#include <random>

class VariantManager {
public:
    VariantManager();
    void addExperiment(const std::string& name, const std::map<std::string, float>& variants);
    std::string selectVariant(const std::string& experiment_name);

private:
    std::map<std::string, std::map<std::string, float>> experiments;
    std::mt19937 gen;
};
