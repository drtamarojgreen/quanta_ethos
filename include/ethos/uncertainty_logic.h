#pragma once

#include <string>

class UncertaintyLogic {
public:
    UncertaintyLogic(float threshold);
    std::string processResponse(const std::string& response, float confidence);

private:
    float threshold;
};
