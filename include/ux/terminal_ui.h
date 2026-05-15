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
    void updateTheme();

    ConfigEngine& config_;
    std::vector<std::unique_ptr<ITerminalView>> views_;
    std::map<std::string, std::string> explanations_;
    int activeViewIndex_ = 0;
    bool running_ = true;

    // Theme variables
    std::string colorHeader_ = "\033[1;37;44m";
    std::string colorActive_ = "\033[1;30;47m";
    std::string colorNormal_ = "\033[0m";
    std::string colorAccent_ = "\033[1;34m";
