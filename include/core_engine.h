#pragma once

#include <string>

class CoreEngine {
public:
    CoreEngine();
    std::string generate_response(const std::string& prompt);
};
