#pragma once

#include <string>

class IModelBackend {
public:
    virtual ~IModelBackend() = default;
    virtual std::string run_model(const std::string& input) = 0;
};
