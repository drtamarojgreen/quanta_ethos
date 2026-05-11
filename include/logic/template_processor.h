#pragma once

#include <string>
#include <map>

class TemplateProcessor {
public:
    TemplateProcessor();
    std::string process(const std::string& templ, const std::map<std::string, std::string>& vars);
};
