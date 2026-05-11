#pragma once

#include <string>

class PiiRedactor {
public:
    PiiRedactor();
    std::string redact(const std::string& text);
};
