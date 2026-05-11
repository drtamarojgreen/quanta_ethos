#include "input_sanitizer.h"
#include <algorithm>

InputSanitizer::InputSanitizer() {}

std::string InputSanitizer::sanitize(const std::string& input) {
    std::string s = input;
    // Remove potentially dangerous characters for a CLI/shell context
    s.erase(std::remove(s.begin(), s.end(), ';'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '`'), s.end());
    return s;
}

bool InputSanitizer::isValid(const std::string& input) {
    return !input.empty();
}
