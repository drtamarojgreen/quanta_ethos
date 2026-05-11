#pragma once

#include <string>
#include <vector>

struct BiasResult {
    bool detected;
    std::string category;
    std::string suggestion;
};

class BiasScanner {
public:
    BiasScanner();
    BiasResult scan(const std::string& text);
    std::string mitigate(const std::string& text, const BiasResult& bias);
};
