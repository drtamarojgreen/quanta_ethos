#include "config_engine.h"
#include <fstream>
#include <sstream>

ConfigEngine::ConfigEngine() {}

void ConfigEngine::load(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
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
