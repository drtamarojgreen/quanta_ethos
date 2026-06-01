#pragma once

#include <string>
#include <map>

class UserSettings {
public:
    UserSettings();
    void setPreference(const std::string& key, const std::string& value);
    std::string getPreference(const std::string& key) const;

    int getKeybinding(const std::string& action) const;
    void setKeybinding(const std::string& action, int key);

private:
    std::map<std::string, std::string> prefs;
    std::map<std::string, int> keybindings;
};
