#pragma once

#include <string>
#include "logic/config_engine.h"

class ITerminalView {
public:
    virtual ~ITerminalView() = default;
    virtual void render(const ConfigEngine& config, int startCol) const = 0;
    virtual void handleInput(int input, ConfigEngine& config) = 0;
    virtual std::string getTitle() const = 0;
    virtual bool isVisible(const ConfigEngine& config) const = 0;
};
