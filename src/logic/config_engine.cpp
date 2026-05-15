#include "config_engine.h"
#include <fstream>
#include <sstream>

ConfigEngine::ConfigEngine() {}

void ConfigEngine::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                settings[key] = value;
            }
        }
    }
}

void ConfigEngine::save(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) return;
    file << "# PrismQuanta Configuration - Saved\n\n";
    for (auto const& [key, val] : settings) {
        file << key << "=" << val << "\n";
    }
}

std::string ConfigEngine::get(const std::string& key, const std::string& default_val) const {
    auto it = settings.find(key);
    return it != settings.end() ? it->second : default_val;
}

int ConfigEngine::getInt(const std::string& key, int default_val) const {
    auto it = settings.find(key);
    return it != settings.end() ? std::stoi(it->second) : default_val;
}

float ConfigEngine::getFloat(const std::string& key, float default_val) const {
    auto it = settings.find(key);
    return it != settings.end() ? std::stof(it->second) : default_val;
}

void ConfigEngine::set(const std::string& key, const std::string& value) {
    settings[key] = value;
}
