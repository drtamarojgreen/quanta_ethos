#include "ux/terminal_ui.h"
#include "ux/context_help.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <iterator>
#include <vector>
#include <cstdio>
#include <chrono>
#include <cctype>
#include <map>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

TerminalUI::TerminalUI(ConfigEngine& config, UserSettings& settings, HealthMonitor& monitor)
    : config_(config), settings_(settings), monitor_(monitor) {
    loadExplanations();
    updateTheme();

    // Register basic commands
    registerCommand("quit", [this]() { running_ = false; });
    registerCommand("theme_emerald", [this]() { config_.set("ui.theme", "emerald"); updateTheme(); });
    registerCommand("theme_amber", [this]() { config_.set("ui.theme", "amber"); updateTheme(); });
    registerCommand("theme_hc", [this]() { config_.set("ui.theme", "high_contrast"); updateTheme(); });
    registerCommand("theme_cb", [this]() { config_.set("ui.theme", "colorblind_safe"); updateTheme(); });
    registerCommand("theme_default", [this]() { config_.set("ui.theme", "default"); updateTheme(); });
    registerCommand("help", [this]() { cheatSheetOpen_ = true; });
    registerCommand("unicode_on", [this]() { config_.set("ui.unicode", "true"); });
    registerCommand("unicode_off", [this]() { config_.set("ui.unicode", "false"); });

    // Layout commands
    registerCommand("layout_default", [this]() { layout_ = LayoutPreset::DEFAULT; });
    registerCommand("layout_grid", [this]() { layout_ = LayoutPreset::GRID; });
    registerCommand("layout_focus", [this]() { layout_ = LayoutPreset::FOCUS; });

    // Workspace commands
    for (int i = 0; i < 4; ++i) {
        registerCommand("ws" + std::to_string(i+1), [this, i]() { activeWorkspace_ = i; });
    }

    // Typography commands
    registerCommand("density_dense", [this]() { config_.set("ui.density", "dense"); });
    registerCommand("density_comfortable", [this]() { config_.set("ui.density", "comfortable"); });

    // Security commands
    registerCommand("lock", [this]() { locked_ = true; pinAttempt_.clear(); });
    registerCommand("wipe_data", [this]() {
        confirmPending_ = true;
        pendingAction_ = []() {
            std::filesystem::remove_all("data/logs");
            std::filesystem::remove_all("data/exports");
        };
    });

    // Persistent History Load
    std::ifstream hfile("data/cmd_history.txt");
    std::string hline;
    while (std::getline(hfile, hline)) commandHistory_.push_back(hline);
}

void TerminalUI::addView(std::unique_ptr<ITerminalView> view) {
    views_.push_back(std::move(view));
}

void TerminalUI::registerCommand(const std::string& name, std::function<void()> action) {
    commands_[name] = action;
}

void TerminalUI::addNotification(const std::string& message, Severity severity) {
    notifications_.push_back({message, severity, std::chrono::system_clock::now()});
    if (notifications_.size() > 5) notifications_.erase(notifications_.begin());
}

void TerminalUI::loadExplanations() {
    std::ifstream file("data/step_explanations.xml");
    if (!file.is_open()) return;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::regex stepRegex("<Step title=\"([^\"]*)\">\\s*<Explanation>([\\s\\S]*?)</Explanation>");
    
    auto begin = std::sregex_iterator(content.begin(), content.end(), stepRegex);
    auto end = std::sregex_iterator();

    for (auto i = begin; i != end; ++i) {
        explanations_[(*i)[1].str()] = (*i)[2].str();
    }
}

void TerminalUI::updateTheme() {
    std::string theme = config_.get("ui.theme", "default");
    if (theme == "emerald") {
        colorHeader_ = "\033[1;37;42m";
        colorActive_ = "\033[1;30;46m";
        colorAccent_ = "\033[1;32m";
    } else if (theme == "amber") {
        colorHeader_ = "\033[1;30;43m";
        colorActive_ = "\033[1;37;41m";
        colorAccent_ = "\033[1;33m";
    } else if (theme == "high_contrast") {
        colorHeader_ = "\033[1;37;40m";
        colorActive_ = "\033[1;30;47m";
        colorAccent_ = "\033[1;37m";
    } else if (theme == "colorblind_safe") {
        colorHeader_ = "\033[1;37;44m"; // Blue
        colorActive_ = "\033[1;30;43m"; // Yellow
        colorAccent_ = "\033[1;36m"; // Cyan
    } else { // default
        colorHeader_ = "\033[1;37;44m";
        colorActive_ = "\033[1;30;47m";
        colorAccent_ = "\033[1;34m";
    }
}

void TerminalUI::drawLayout() {
    updateTheme();
    std::cout << "\033[2J\033[H";

    if (locked_) {
        std::cout << "\033[10;30H\033[1;37;41m SESSION LOCKED \033[0m";
        std::cout << "\033[12;25HEnter PIN: " << std::string(pinAttempt_.length(), '*') << "_";
        std::cout << "\033[22;1H" << colorActive_ << " [Esc] to Quit Session " << "\033[0m";
        std::cout << std::flush;
        return;
    }

    // Draw header with Workspace tabs
    bool isProd = config_.get("system.env", "dev") == "production";
    if (isProd) std::cout << "\033[1;37;41m PRODUCTION \033[0m ";

    std::cout << colorHeader_ << " PrismQuanta ";
    for (int i = 0; i < 4; ++i) {
        if (i == activeWorkspace_) std::cout << colorActive_ << " WS" << (i + 1) << " \033[0m" << colorHeader_;
        else std::cout << " [" << (i + 1) << "]";
    }
    std::cout << "\033[K\033[0m\n";

    int height = 19;
    int width = 80;

    bool dense = config_.get("ui.density", "comfortable") == "dense";
    if (dense) height = 22; // More lines if dense

    std::vector<ITerminalView*> visibleViews;
    for (auto& v : views_) if (v->isVisible(config_)) visibleViews.push_back(v.get());

    // Draw Breadcrumbs
    std::cout << "\033[1;60H" << colorAccent_ << " Home > ";
    if (!visibleViews.empty()) {
        if (activeViewIndex_ >= (int)visibleViews.size()) activeViewIndex_ = (int)visibleViews.size() - 1;
        std::cout << visibleViews[activeViewIndex_]->getTitle();
    }
    std::cout << "\033[0m";

    // Contextual Help
    ContextHelp help;
    std::string contextualHelp = "";
    if (!visibleViews.empty()) {
        contextualHelp = help.getHelp(visibleViews[activeViewIndex_]->getTitle());
    }

    bool useUnicode = config_.get("ui.unicode", "true") == "true";

    if (!visibleViews.empty()) {
        if (layout_ == LayoutPreset::DEFAULT) {
            int menuWidth = menuWidth_;
            int contentWidth = 30;
            std::string vLine = useUnicode ? "│" : "|";
            for (int i = 0; i < height; ++i) {
                std::cout << "\033[" << (i + 2) << ";1H\033[K";
                if (i < (int)visibleViews.size()) {
                    if (i == activeViewIndex_) std::cout << colorActive_ << " > " << visibleViews[i]->getTitle() << " \033[0m";
                    else std::cout << "   " << visibleViews[i]->getTitle();
                }
                std::cout << "\033[" << (i + 2) << ";" << menuWidth << "H" << vLine;
                std::cout << "\033[" << (i + 2) << ";" << (menuWidth + contentWidth) << "H" << vLine;
            }
            std::cout << "\033[2;" << (menuWidth + 2) << "H";
            visibleViews[activeViewIndex_]->render(config_, menuWidth + 2);

            std::string title = visibleViews[activeViewIndex_]->getTitle();
            if (explanations_.count(title)) {
                std::string exp = explanations_[title];
                int expCol = menuWidth + contentWidth + 2;
                std::cout << "\033[2;" << expCol << "H" << colorAccent_ << "\033[1mExplanation:\033[0m";
                int line = 4;
                std::string word;
                std::stringstream ss(exp);
                std::string currentLine;
                while (ss >> word) {
                    if (currentLine.length() + word.length() + 1 > (size_t)(80 - expCol - 2)) {
                        std::cout << "\033[" << line++ << ";" << expCol << "H" << currentLine;
                        currentLine = word;
                        if (line > height) break;
                    } else {
                        if (!currentLine.empty()) currentLine += " ";
                        currentLine += word;
                    }
                }
                if (line <= height) std::cout << "\033[" << line << ";" << expCol << "H" << currentLine;
            }
            // Render Contextual Help
            std::cout << "\033[" << (height-2) << ";" << (menuWidth + contentWidth + 2) << "H" << colorAccent_ << "Context: " << contextualHelp << "\033[0m";
        } else if (layout_ == LayoutPreset::GRID) {
            int count = std::min((int)visibleViews.size(), 4);
            int cellW = width / 2; int cellH = height / 2;
            for (int i = 0; i < count; ++i) {
                int r = i / 2; int c = i % 2;
                int x = c * cellW + 1; int y = r * cellH + 2;
                std::cout << "\033[" << y << ";" << x << "H" << (i == activeViewIndex_ ? colorActive_ : colorHeader_)
                          << " " << visibleViews[i]->getTitle() << " \033[0m";
                std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
                visibleViews[i]->render(config_, x + 1);
            }
        }
    }

    drawStatusBar();
    drawCommandPalette();
    drawNotifications();
    drawConfirmDialog();
    drawContextMenu();
    drawCheatSheet();
    std::cout << std::flush;
}

void TerminalUI::drawContextMenu() {
    if (!contextMenuOpen_ || activeContextMenu_.empty()) return;
    int startRow = 10; int startCol = 30;
    std::cout << "\033[" << startRow << ";" << startCol << "H\033[48;5;238m\033[37m CONTEXT MENU \033[0m";
    for (size_t i = 0; i < activeContextMenu_.size(); ++i) {
        std::cout << "\033[" << (startRow + i + 1) << ";" << startCol << "H";
        if (i == (size_t)contextMenuIndex_) std::cout << colorActive_ << " > " << activeContextMenu_[i].label << " \033[0m";
        else std::cout << "\033[48;5;238m   " << activeContextMenu_[i].label << " \033[0m";
    }
}

void TerminalUI::drawConfirmDialog() {
    if (!confirmPending_) return;
    std::cout << "\033[10;20H\033[1;37;41m ARE YOU SURE? [Y/N] \033[0m";
}

void TerminalUI::drawCheatSheet() {
    if (!cheatSheetOpen_) return;
    int sr = 5; int sc = 15;
    std::cout << "\033[" << sr << ";" << sc << "H" << colorHeader_ << " KEYBOARD SHORTCUTS \033[0m";
    std::cout << "\033[" << (sr+1) << ";" << sc << "H\033[48;5;236m : - Command Mode  \033[0m";
    std::cout << "\033[" << (sr+2) << ";" << sc << "H\033[48;5;236m j/k - Navigate     \033[0m";
    std::cout << "\033[" << (sr+3) << ";" << sc << "H\033[48;5;236m m - Context Menu  \033[0m";
    std::cout << "\033[" << (sr+4) << ";" << sc << "H\033[48;5;236m ? - This Help     \033[0m";
    std::cout << "\033[" << (sr+5) << ";" << sc << "H\033[48;5;236m q - Quit          \033[0m";
}

void TerminalUI::drawCommandPalette() {
    if (mode_ != InputMode::COMMAND) return;
    int startRow = 5; int startCol = 20;
    std::cout << "\033[" << startRow << ";" << startCol << "H\033[48;5;236m\033[37m COMMAND PALETTE \033[0m";
    std::cout << "\033[" << (startRow+1) << ";" << startCol << "H\033[48;5;236m :" << commandLine_ << "_\033[0m";
    int i = 0;
    for (const auto& [name, action] : commands_) {
        bool match = true; size_t last = 0;
        for (char c : commandLine_) {
            size_t p = name.find(std::tolower(c), last);
            if (p == std::string::npos) { match = false; break; }
            last = p + 1;
        }
        if (match) {
            std::cout << "\033[" << (startRow+2+i) << ";" << startCol << "H\033[48;5;236m  " << name << " \033[0m";
            if (++i > 5) break;
        }
    }
}

void TerminalUI::drawNotifications() {
    int startRow = 2; int startCol = 50;
    auto now = std::chrono::system_clock::now();
    for (const auto& n : notifications_) {
        auto age = std::chrono::duration_cast<std::chrono::seconds>(now - n.timestamp).count();
        if (age > 10) continue;
        std::string color = (n.severity == Severity::CRITICAL) ? "\033[1;37;41m" : (n.severity == Severity::WARNING ? "\033[1;37;43m" : "\033[1;37;44m");
        std::cout << "\033[" << startRow++ << ";" << startCol << "H" << color << " " << n.message << " \033[0m";
    }
}

void TerminalUI::drawStatusBar() {
    int statusLine = 21;
    std::cout << "\033[" << statusLine << ";1H" << colorHeader_;
    std::string modeStr = (mode_ == InputMode::COMMAND) ? " COMMAND " : (mode_ == InputMode::INSERT ? " INSERT " : " NORMAL ");
    std::cout << modeStr << " | ";
    if (mode_ == InputMode::COMMAND) {
        std::cout << ":" << commandLine_ << " ";
    } else {
        auto history = monitor_.getHistory();
        double lat = history.empty() ? 0.0 : history.back().latency_ms;
        std::string health = monitor_.isHealthy() ? "OK" : "ERROR";
        std::cout << "v1.0 | Health: " << health << " | Latency: " << std::fixed << std::setprecision(1) << lat << "ms ";
    }
    // Power User Tip
    static std::vector<std::string> tips = {
        "Try ':', then type 'layout_grid'",
        "Press 'm' for context actions",
        "Use 'j' and 'k' to navigate",
        "Type ':lock' to secure session",
        "Use '>' and '<' to resize panes"
    };
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << " | Tip: " << tips[(seconds / 10) % tips.size()] << " ";
    std::cout << "\033[K\033[0m";
}

void TerminalUI::handleInput() {
#ifdef _WIN32
    int ch = _getch();
#else
    int ch = getchar();
#endif
    if (locked_) {
        std::string pin = config_.get("security.pin", "1234");
        if (ch == 10 || ch == 13) { if (pinAttempt_ == pin) locked_ = false; pinAttempt_.clear(); }
        else if (ch == 127 || ch == 8) { if (!pinAttempt_.empty()) pinAttempt_.pop_back(); }
        else if (std::isdigit(ch)) pinAttempt_ += (char)ch;
        return;
    }
    if (cheatSheetOpen_) { cheatSheetOpen_ = false; return; }
    if (contextMenuOpen_) {
        if (ch == 27) { contextMenuOpen_ = false; return; }
        if (ch == 10 || ch == 13) { if (contextMenuIndex_ < (int)activeContextMenu_.size()) activeContextMenu_[contextMenuIndex_].action(); contextMenuOpen_ = false; return; }
        if (ch == 'k' || ch == 'K' || ch == 65) { if (contextMenuIndex_ > 0) contextMenuIndex_--; return; }
        if (ch == 'j' || ch == 'J' || ch == 66) { if (contextMenuIndex_ < (int)activeContextMenu_.size() - 1) contextMenuIndex_++; return; }
    }
    if (confirmPending_) {
        if (ch == 'y' || ch == 'Y') { if (pendingAction_) pendingAction_(); confirmPending_ = false; }
        else if (ch == 'n' || ch == 'N' || ch == 27) confirmPending_ = false;
        return;
    }
    if (ch == 27) {
#ifndef _WIN32
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0); fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        int n1 = getchar();
        if (n1 == '[') {
            int n2 = getchar(); fcntl(STDIN_FILENO, F_SETFL, flags);
            if (n2 == 'A') { if (activeViewIndex_ > 0) activeViewIndex_--; return; }
            if (n2 == 'B') {
                std::vector<ITerminalView*> vv; for (auto& v : views_) if (v->isVisible(config_)) vv.push_back(v.get());
                if (activeViewIndex_ < (int)vv.size() - 1) activeViewIndex_++; return;
            }
        }
        fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
        mode_ = InputMode::NORMAL; commandLine_.clear(); return;
    }
    std::vector<ITerminalView*> visibleViews; for (auto& v : views_) if (v->isVisible(config_)) visibleViews.push_back(v.get());
    if (mode_ == InputMode::NORMAL) {
        if (ch == settings_.getKeybinding("command")) { mode_ = InputMode::COMMAND; commandLine_.clear(); }
        else if (ch == 'm' || ch == 'M') { if (!visibleViews.empty()) { activeContextMenu_ = visibleViews[activeViewIndex_]->getContextMenu(); if (!activeContextMenu_.empty()) { contextMenuOpen_ = true; contextMenuIndex_ = 0; } } }
        else if (ch == 'i' || ch == 'I') { mode_ = InputMode::INSERT; }
        else if (ch == '?') { cheatSheetOpen_ = true; }
        else if (ch == settings_.getKeybinding("nav_down") || ch == std::tolower(settings_.getKeybinding("nav_down"))) { if (activeViewIndex_ < (int)visibleViews.size() - 1) activeViewIndex_++; }
        else if (ch == settings_.getKeybinding("nav_up") || ch == std::tolower(settings_.getKeybinding("nav_up"))) { if (activeViewIndex_ > 0) activeViewIndex_--; }
        else if (ch == 'L' || ch == '>') { if (menuWidth_ < 40) menuWidth_++; }
        else if (ch == 'H' || ch == '<') { if (menuWidth_ > 10) menuWidth_--; }
        else if (ch == settings_.getKeybinding("quit") || ch == std::tolower(settings_.getKeybinding("quit"))) { running_ = false; }
        else if (!visibleViews.empty()) visibleViews[activeViewIndex_]->handleInput(ch, config_);
    } else if (mode_ == InputMode::COMMAND) {
        if (ch == 10 || ch == 13) {
            if (!commandLine_.empty()) { commandHistory_.push_back(commandLine_); historyIndex_ = -1; std::ofstream hfile("data/cmd_history.txt", std::ios::app); hfile << commandLine_ << "\n"; }
            if (commands_.count(commandLine_)) commands_[commandLine_]();
            mode_ = InputMode::NORMAL; commandLine_.clear();
        } else if (ch == 127 || ch == 8) { if (!commandLine_.empty()) commandLine_.pop_back(); else mode_ = InputMode::NORMAL; }
        else if (ch == 27) {
            int n1 = getchar();
            if (n1 == '[') {
                int n2 = getchar();
                if (n2 == 'A') { // Up
                    if (!commandHistory_.empty()) {
                        if (historyIndex_ == -1) historyIndex_ = commandHistory_.size() - 1;
                        else if (historyIndex_ > 0) historyIndex_--;
                        commandLine_ = commandHistory_[historyIndex_];
                    }
                } else if (n2 == 'B') { // Down
                    if (historyIndex_ != -1 && historyIndex_ < (int)commandHistory_.size() - 1) {
                        historyIndex_++;
                        commandLine_ = commandHistory_[historyIndex_];
                    } else {
                        historyIndex_ = -1;
                        commandLine_.clear();
                    }
                }
            } else {
                mode_ = InputMode::NORMAL;
                commandLine_.clear();
            }
        } else if (std::isprint(ch)) commandLine_ += (char)ch;
    } else if (mode_ == InputMode::INSERT) {
        if (!visibleViews.empty()) visibleViews[activeViewIndex_]->handleInput(ch, config_);
    }
}

void TerminalUI::run() {
#ifndef _WIN32
    struct termios oldt, newt; tcgetattr(STDIN_FILENO, &oldt); newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif
    std::cout << "\033[?1000h\033[?1003h\033[?1015h\033[?1006h\033[?25l";
    while (running_) { drawLayout(); handleInput(); }
    std::cout << "\033[?1000l\033[?1003l\033[?1015l\033[?1006l\033[?25h";
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}
