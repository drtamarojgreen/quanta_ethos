#include "user_settings.h"

UserSettings::UserSettings() {
    prefs["language"] = "en";
    prefs["verbosity"] = "normal";

    // Default keybindings
    keybindings["nav_up"] = 'k';
    keybindings["nav_down"] = 'j';
    keybindings["quit"] = 'q';
    keybindings["command"] = ':';
}

void UserSettings::setPreference(const std::string& k, const std::string& v) {
    prefs[k] = v;
}

std::string UserSettings::getPreference(const std::string& k) const {
    auto it = prefs.find(k);
    return it != prefs.end() ? it->second : "";
}

int UserSettings::getKeybinding(const std::string& action) const {
    auto it = keybindings.find(action);
    return it != keybindings.end() ? it->second : -1;
}

void UserSettings::setKeybinding(const std::string& action, int key) {
    keybindings[action] = key;
}
