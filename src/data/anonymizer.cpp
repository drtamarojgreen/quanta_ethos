#include "anonymizer.h"
#include <regex>

Anonymizer::Anonymizer() {}

std::string Anonymizer::anonymize(const std::string& input) {
    std::string result = input;
    // Replace names like "User: Name" with "User: [ANONYMOUS]"
    std::regex name_regex(R"(User: [a-zA-Z]+)");
    result = std::regex_replace(result, name_regex, "User: [ANONYMOUS]");
    return result;
}
