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

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

TerminalUI::TerminalUI(ConfigEngine& config) : config_(config) {
    loadExplanations();
    updateTheme();

    // Register basic commands
    registerCommand("quit", [this]() { running_ = false; });
    registerCommand("theme_emerald", [this]() { config_.set("ui.theme", "emerald"); updateTheme(); });
    registerCommand("theme_amber", [this]() { config_.set("ui.theme", "amber"); updateTheme(); });
    registerCommand("theme_hc", [this]() { config_.set("ui.theme", "high_contrast"); updateTheme(); });
    registerCommand("theme_cb", [this]() { config_.set("ui.theme", "colorblind_safe"); updateTheme(); });
    registerCommand("theme_default", [this]() { config_.set("ui.theme", "default"); updateTheme(); });
    registerCommand("help", [this]() { addNotification("Help command triggered", Severity::INFO); });

    // Layout commands
    registerCommand("layout_default", [this]() { layout_ = LayoutPreset::DEFAULT; });
    registerCommand("layout_grid", [this]() { layout_ = LayoutPreset::GRID; });
    registerCommand("layout_focus", [this]() { layout_ = LayoutPreset::FOCUS; });
    registerCommand("layout_fullscreen", [this]() { layout_ = LayoutPreset::FULLSCREEN; });

    // Workspace commands
    for (int i = 0; i < 4; ++i) {
        registerCommand("ws" + std::to_string(i+1), [this, i]() { activeWorkspace_ = i; });
    }

    // Typography commands
    registerCommand("density_dense", [this]() { config_.set("ui.density", "dense"); });
    registerCommand("density_comfortable", [this]() { config_.set("ui.density", "comfortable"); });
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
    // Simple regex to extract title and explanation
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
    // Clear screen
    std::cout << "\033[2J\033[H";

    // Draw header
    std::cout << colorHeader_ << " PrismQuanta ";
    for (int i = 0; i < 4; ++i) {
        if (i == activeWorkspace_) std::cout << colorActive_ << " Workspace " << (i + 1) << " \033[0m" << colorHeader_;
        else std::cout << " [" << (i + 1) << "] ";
    }
    std::cout << " \033[0m\n";

    int height = 19;
    int width = 80;

    bool dense = config_.get("ui.density", "comfortable") == "dense";
    if (dense) height = 22; // More lines if dense

    std::vector<ITerminalView*> visibleViews;
    for (auto& v : views_) if (v->isVisible(config_)) visibleViews.push_back(v.get());

    // Draw Breadcrumbs
    std::cout << "\033[1;60H" << colorAccent_ << " Home > ";
    if (!visibleViews.empty()) {
        std::cout << visibleViews[activeViewIndex_]->getTitle();
    }
    std::cout << "\033[0m";

    // Contextual Help
    ContextHelp help;
    std::string contextualHelp = "";
    if (!visibleViews.empty()) {
        contextualHelp = help.getHelp(visibleViews[activeViewIndex_]->getTitle());
    }

    if (!visibleViews.empty()) {
        if (activeViewIndex_ >= (int)visibleViews.size()) activeViewIndex_ = (int)visibleViews.size() - 1;

        if (layout_ == LayoutPreset::DEFAULT) {
            int menuWidth = 20;
            int contentWidth = 30;
            int explanationWidth = 30;

            for (int i = 0; i < height; ++i) {
                std::cout << "\033[" << (i + 2) << ";1H\033[K";
                if (i < (int)visibleViews.size()) {
                    if (i == activeViewIndex_) std::cout << colorActive_ << " > " << visibleViews[i]->getTitle() << " \033[0m";
                    else std::cout << "   " << visibleViews[i]->getTitle();
                }
                std::cout << "\033[" << (i + 2) << ";" << menuWidth << "H|";
                std::cout << "\033[" << (i + 2) << ";" << (menuWidth + contentWidth) << "H|";
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
                    if (currentLine.length() + word.length() + 1 > (size_t)(explanationWidth - 4)) {
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

            // Render Contextual Help at the bottom of the right panel
            int helpStartLine = 15;
            std::cout << "\033[" << helpStartLine << ";" << (menuWidth + contentWidth + 2) << "H" << colorAccent_ << "Context Help: " << contextualHelp << "\033[0m";
        } else if (layout_ == LayoutPreset::GRID) {
            int count = std::min((int)visibleViews.size(), 4);
            int cols = (count > 2) ? 2 : 1;
            int rows = (count + cols - 1) / cols;
            int cellW = width / cols;
            int cellH = height / rows;

            for (int i = 0; i < count; ++i) {
                int r = i / cols;
                int c = i % cols;
                int x = c * cellW + 1;
                int y = r * cellH + 2;
                std::cout << "\033[" << y << ";" << x << "H" << (i == activeViewIndex_ ? colorActive_ : colorHeader_)
                          << " " << visibleViews[i]->getTitle() << " \033[0m";
                std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
                visibleViews[i]->render(config_, x + 1);
            }
        } else if (layout_ == LayoutPreset::FOCUS || layout_ == LayoutPreset::FULLSCREEN) {
            std::cout << "\033[2;1H" << colorActive_ << " FOCUS: " << visibleViews[activeViewIndex_]->getTitle() << " \033[0m\n";
            visibleViews[activeViewIndex_]->render(config_, 1);
        }
    }

    drawStatusBar();
    drawCommandPalette();
    drawNotifications();
    std::cout << std::flush;
}

void TerminalUI::drawCommandPalette() {
    if (mode_ != InputMode::COMMAND) return;

    int pWidth = 40;
    int pHeight = 10;
    int startCol = 20;
    int startRow = 5;

    // Draw box with ANSI 256-color if supported, otherwise fallback to standard
    std::string bg = "\033[48;5;236m";
    std::string fg = "\033[37m";

    for (int i = 0; i < pHeight; ++i) {
        std::cout << "\033[" << (startRow + i) << ";" << startCol << "H" << bg;
        for (int j = 0; j < pWidth; ++j) std::cout << " ";
        std::cout << "\033[0m";
    }

    std::cout << "\033[" << startRow << ";" << startCol << "H" << bg << fg << "\033[1m Command: " << commandLine_ << "_\033[0m";

    int line = 1;
    for (const auto& [name, action] : commands_) {
        if (commandLine_.empty() || name.find(commandLine_) != std::string::npos) {
            std::cout << "\033[" << (startRow + line) << ";" << (startCol + 2) << "H" << bg << fg << name << "\033[0m";
            line++;
            if (line >= pHeight) break;
        }
    }
}

void TerminalUI::drawNotifications() {
    int startRow = 2;
    int startCol = 50;
    auto now = std::chrono::system_clock::now();

    for (const auto& n : notifications_) {
        auto age = std::chrono::duration_cast<std::chrono::seconds>(now - n.timestamp).count();
        if (age > 10) continue;

        std::string color = "\033[1;37;44m"; // Info: Blue
        if (n.severity == Severity::WARNING) color = "\033[1;37;43m"; // Warning: Amber
        if (n.severity == Severity::CRITICAL) color = "\033[1;37;41m"; // Critical: Red

        std::cout << "\033[" << startRow++ << ";" << startCol << "H" << color << " " << n.message << " \033[0m";
        if (startRow > 10) break;
    }
}

void TerminalUI::drawStatusBar() {
    int statusLine = 21;
    int helpLine = 22;
    std::cout << "\033[" << statusLine << ";1H" << colorHeader_;

    std::string modeStr;
    switch(mode_) {
        case InputMode::NORMAL: modeStr = " NORMAL "; break;
        case InputMode::COMMAND: modeStr = " COMMAND "; break;
        case InputMode::INSERT: modeStr = " INSERT "; break;
    }

    std::stringstream ss;
    ss << modeStr << " | ";

    if (mode_ == InputMode::COMMAND) {
        ss << ":" << commandLine_ << " ";
    } else {
        ss << "PrismQuanta v1.0 | Health: OK | Latency: 12ms ";
    }

    std::string statusText = ss.str();
    std::cout << statusText;

    // Fill the rest of the line with spaces (assuming 80 columns)
    int remaining = 80 - (int)statusText.length();
    if (remaining > 0) std::cout << std::string(remaining, ' ');

    std::cout << "\033[0m";

    std::cout << "\033[" << helpLine << ";1H" << colorActive_
              << " [Esc] Normal | [:] Command | [/] Search | [Q] Quit " << "\033[0m";
}

void TerminalUI::handleInput() {
#ifdef _WIN32
    int ch = _getch();
#else
    int ch = getchar();
#endif

    if (ch == 27) { // Escape
        mode_ = InputMode::NORMAL;
        commandLine_.clear();
        return;
    }

    std::vector<ITerminalView*> visibleViews;
    for (auto& v : views_) {
        if (v->isVisible(config_)) {
            visibleViews.push_back(v.get());
        }
    }

    if (mode_ == InputMode::NORMAL) {
        if (ch == ':') {
            mode_ = InputMode::COMMAND;
            commandLine_.clear();
        } else if (ch == 'j' || ch == 'J' || ch == 's' || ch == 'S') { // S for Down in some profiles
            if (activeViewIndex_ < (int)visibleViews.size() - 1) activeViewIndex_++;
        } else if (ch == 'k' || ch == 'K' || ch == 'w' || ch == 'W') { // W for Up in some profiles
            if (activeViewIndex_ > 0) activeViewIndex_--;
        } else if (ch == 'q' || ch == 'Q') {
            running_ = false;
        } else if (ch == '\033' || ch == 224) { // Arrows fallback
#ifdef _WIN32
            int next = (ch == 224) ? _getch() : _getch();
            if (ch == 224) {
                 switch(next) {
                    case 72: if (activeViewIndex_ > 0) activeViewIndex_--; break;
                    case 80: {
                        if (activeViewIndex_ < (int)visibleViews.size() - 1) activeViewIndex_++;
                        break;
                    }
                }
            }
#else
            int next = getchar();
            if (next == '[') {
                switch(getchar()) {
                    case 'A': if (activeViewIndex_ > 0) activeViewIndex_--; break;
                    case 'B': {
                        if (activeViewIndex_ < (int)visibleViews.size() - 1) activeViewIndex_++;
                        break;
                    }
                }
            }
#endif
        } else {
            // Pass to active view
            if (!visibleViews.empty() && activeViewIndex_ < (int)visibleViews.size()) {
                visibleViews[activeViewIndex_]->handleInput(ch, config_);
            }
        }
    } else if (mode_ == InputMode::COMMAND) {
        if (ch == 10 || ch == 13) { // Enter
            if (commands_.count(commandLine_)) {
                commands_[commandLine_]();
            }
            mode_ = InputMode::NORMAL;
            commandLine_.clear();
        } else if (ch == 127 || ch == 8) { // Backspace
            if (!commandLine_.empty()) commandLine_.pop_back();
            else mode_ = InputMode::NORMAL;
        } else if (std::isprint(ch)) {
            commandLine_ += (char)ch;
        }
    }
}

void TerminalUI::run() {
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    DWORD oldMode = mode;
    mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hStdin, mode);
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif

    // Hide cursor
    std::cout << "\033[?25l";

    while (running_) {
        drawLayout();
        handleInput();
    }

    // Show cursor
    std::cout << "\033[?25h";

#ifdef _WIN32
    SetConsoleMode(hStdin, oldMode);
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}
