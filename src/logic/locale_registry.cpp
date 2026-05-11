#include "locale_registry.h"

LocaleRegistry::LocaleRegistry() : current_locale("en_US") {}

void LocaleRegistry::setLocale(const std::string& locale) {
    current_locale = locale;
}

void LocaleRegistry::addTranslation(const std::string& locale, const std::string& key, const std::string& value) {
    translations[locale][key] = value;
}

std::string LocaleRegistry::translate(const std::string& key) const {
    auto loc_it = translations.find(current_locale);
    if (loc_it != translations.end()) {
        auto key_it = loc_it->second.find(key);
        if (key_it != loc_it->second.end()) {
            return key_it->second;
        }
    }
    return key;
}
