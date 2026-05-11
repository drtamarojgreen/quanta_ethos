#pragma once

#include <string>
#include <vector>

class ApiBatching {
public:
    ApiBatching();
    std::vector<std::string> processBatch(const std::vector<std::string>& prompts);
};
