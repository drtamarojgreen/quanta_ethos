#pragma once

#include <string>
#include <map>

class ConfigEngine {
public:
    ConfigEngine();
    void load(const std::string& path);
    void save(const std::string& path);
    std::string get(const std::string& key, const std::string& default_val = "") const;
    int getInt(const std::string& key, int default_val = 0) const;
    float getFloat(const std::string& key, float default_val = 0.0f) const;
    void set(const std::string& key, const std::string& value);

private:
    std::map<std::string, std::string> settings;
};
