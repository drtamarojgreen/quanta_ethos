#include "user_settings.h"

UserSettings::UserSettings() {
    prefs["language"] = "en";
    prefs["verbosity"] = "normal";
}

void UserSettings::setPreference(const std::string& k, const std::string& v) {
    prefs[k] = v;
}

std::string UserSettings::getPreference(const std::string& k) const {
    auto it = prefs.find(k);
    return it != prefs.end() ? it->second : "";
}
