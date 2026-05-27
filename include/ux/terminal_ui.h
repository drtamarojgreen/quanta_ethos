#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>
#include "terminal_view.h"
#include "logic/config_engine.h"

enum class InputMode {
    NORMAL,
    COMMAND,
    INSERT
};

enum class LayoutPreset {
    DEFAULT,
    GRID,
    FOCUS,
    FULLSCREEN
};

enum class Severity {
    INFO,
    WARNING,
    CRITICAL
};

struct Notification {
    std::string message;
    Severity severity;
    std::chrono::system_clock::time_point timestamp;
};

struct Pane {
    int x, y, w, h;
    ITerminalView* view;
};

class TerminalUI {
public:
    TerminalUI(ConfigEngine& config);
    void run();
    void addView(std::unique_ptr<ITerminalView> view);
    void registerCommand(const std::string& name, std::function<void()> action);
    void addNotification(const std::string& message, Severity severity = Severity::INFO);

private:
    void drawLayout();
    void drawStatusBar();
    void drawCommandPalette();
    void drawNotifications();
    void handleInput();
    void loadExplanations();
    void updateTheme();

    ConfigEngine& config_;
    std::vector<std::unique_ptr<ITerminalView>> views_;
    std::map<std::string, std::string> explanations_;
    int activeViewIndex_ = 0;
    int activeWorkspace_ = 0;
    bool running_ = true;
    InputMode mode_ = InputMode::NORMAL;
    LayoutPreset layout_ = LayoutPreset::DEFAULT;
    std::string commandLine_;
    std::map<std::string, std::function<void()>> commands_;
    std::vector<Notification> notifications_;

    // Theme variables
    std::string colorHeader_ = "\033[1;37;44m";
    std::string colorActive_ = "\033[1;30;47m";
    std::string colorNormal_ = "\033[0m";
    std::string colorAccent_ = "\033[1;34m";
};
