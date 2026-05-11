#pragma once

#include <string>
#include <map>

class UserSettings {
public:
    UserSettings();
    void setPreference(const std::string& key, const std::string& value);
    std::string getPreference(const std::string& key) const;

private:
    std::map<std::string, std::string> prefs;
};
