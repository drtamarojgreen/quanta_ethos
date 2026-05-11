#pragma once

#include <string>
#include <map>

class LocaleRegistry {
public:
    LocaleRegistry();
    void setLocale(const std::string& locale);
    void addTranslation(const std::string& locale, const std::string& key, const std::string& value);
    std::string translate(const std::string& key) const;

private:
    std::string current_locale;
    std::map<std::string, std::map<std::string, std::string>> translations;
};
