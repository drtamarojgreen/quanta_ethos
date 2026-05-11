#pragma once

#include <string>

class InputSanitizer {
public:
    InputSanitizer();
    std::string sanitize(const std::string& input);
    bool isValid(const std::string& input);
};
