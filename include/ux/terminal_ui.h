#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>
#include "terminal_view.h"
#include "user_settings.h"
#include "logic/config_engine.h"
#include "model/health_monitor.h"

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
    TerminalUI(ConfigEngine& config, UserSettings& settings, HealthMonitor& monitor);
    void run();
    void addView(std::unique_ptr<ITerminalView> view);
    void registerCommand(const std::string& name, std::function<void()> action);
    void addNotification(const std::string& message, Severity severity = Severity::INFO);

private:
    void drawLayout();
    void drawStatusBar();
    void drawCommandPalette();
    void drawNotifications();
    void drawConfirmDialog();
    void drawContextMenu();
    void drawCheatSheet();
    void handleInput();
    void loadExplanations();
    void updateTheme();

    ConfigEngine& config_;
    UserSettings& settings_;
    HealthMonitor& monitor_;
    std::vector<std::unique_ptr<ITerminalView>> views_;
    std::map<std::string, std::string> explanations_;
    int activeViewIndex_ = 0;
    int activeWorkspace_ = 0;
    int menuWidth_ = 20;
    bool running_ = true;
    bool locked_ = false;
    bool confirmPending_ = false;
    bool contextMenuOpen_ = false;
    bool cheatSheetOpen_ = false;
    int contextMenuIndex_ = 0;
    std::vector<ContextMenuItem> activeContextMenu_;
    std::function<void()> pendingAction_;
    std::string pinAttempt_;
    InputMode mode_ = InputMode::NORMAL;
    LayoutPreset layout_ = LayoutPreset::DEFAULT;
    std::string commandLine_;
    std::vector<std::string> commandHistory_;
    int historyIndex_ = -1;
    std::map<std::string, std::function<void()>> commands_;
    std::vector<Notification> notifications_;

public:
    // Diagnostic methods for SDD testing
    int getCommandCount() const { return commands_.size(); }
    int getActiveWorkspace() const { return activeWorkspace_; }
    int getNotificationCount() const { return notifications_.size(); }
    InputMode getMode() const { return mode_; }

private:
    // Theme variables
    std::string colorHeader_ = "\033[1;37;44m";
    std::string colorActive_ = "\033[1;30;47m";
    std::string colorNormal_ = "\033[0m";
    std::string colorAccent_ = "\033[1;34m";
};
