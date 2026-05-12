#include "input_sanitizer.h"
#include <algorithm>

InputSanitizer::InputSanitizer() {}

std::string InputSanitizer::sanitize(const std::string& input) {
    std::string s = input;

    const auto command_separator = s.find_first_of(";|&`");
    if (command_separator != std::string::npos) {
        s = s.substr(0, command_separator);
    }

    while (!s.empty() && s.back() == ' ') {
        s.pop_back();
    }

    return s;
}

bool InputSanitizer::isValid(const std::string& input) {
    return !input.empty();
}
