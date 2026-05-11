#pragma once

#include <string>

class Anonymizer {
public:
    Anonymizer();
    std::string anonymize(const std::string& input);
};
