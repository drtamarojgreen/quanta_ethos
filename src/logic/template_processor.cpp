#include "template_processor.h"
#include <regex>

TemplateProcessor::TemplateProcessor() {}

std::string TemplateProcessor::process(const std::string& templ, const std::map<std::string, std::string>& vars) {
    std::string result = templ;
    for (const auto& pair : vars) {
        std::regex pattern("\\{\\{" + pair.first + "\\}\\}");
        result = std::regex_replace(result, pattern, pair.second);
    }
    return result;
}
