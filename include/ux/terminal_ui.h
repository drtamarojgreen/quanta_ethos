#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "terminal_view.h"
#include "logic/config_engine.h"

class TerminalUI {
public:
    TerminalUI(ConfigEngine& config);
    void run();
    void addView(std::unique_ptr<ITerminalView> view);

private:
    void drawLayout();
    void handleInput();
    void loadExplanations();

    ConfigEngine& config_;
    std::vector<std::unique_ptr<ITerminalView>> views_;
    std::map<std::string, std::string> explanations_;
    int activeViewIndex_ = 0;
    bool running_ = true;
};
