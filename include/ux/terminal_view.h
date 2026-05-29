#pragma once

#include <string>
#include <functional>
#include <vector>
#include "logic/config_engine.h"

struct ContextMenuItem {
    std::string label;
    std::function<void()> action;
};

class ITerminalView {
public:
    virtual ~ITerminalView() = default;
    virtual void render(const ConfigEngine& config, int startCol) const = 0;
    virtual void handleInput(int input, ConfigEngine& config) = 0;
    virtual std::string getTitle() const = 0;
    virtual bool isVisible(const ConfigEngine& config) const = 0;
    virtual std::vector<ContextMenuItem> getContextMenu() const { return {}; }
};
