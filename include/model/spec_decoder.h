#pragma once

#include <string>
#include <memory>
#include "plugin_interfaces.h"

class SpeculativeDecoder {
public:
    SpeculativeDecoder(IModelBackend* draft_model, IModelBackend* target_model);
    std::string generate(const std::string& prompt);

private:
    IModelBackend* draft;
    IModelBackend* target;
};
