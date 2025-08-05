#pragma once

#include <string>

class ModelBackend {
public:
    ModelBackend();
    std::string run_model(const std::string& input);
};
