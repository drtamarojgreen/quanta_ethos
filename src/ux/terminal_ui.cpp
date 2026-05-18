#include "ux/terminal_ui.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <iterator>
#include <vector>
#include <cstdio>

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
}

void TerminalUI::addView(std::unique_ptr<ITerminalView> view) {
    views_.push_back(std::move(view));
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
    std::cout << colorHeader_ << " PrismQuanta System Configuration \033[0m\n";

    int height = 20;
    int width = 80;
    int menuWidth = 20; // 25% of 80
    int contentWidth = 30;
    int explanationWidth = 30;

    std::vector<ITerminalView*> visibleViews;
    for (auto& v : views_) {
        if (v->isVisible(config_)) {
            visibleViews.push_back(v.get());
        }
    }

    if (activeViewIndex_ >= (int)visibleViews.size()) {
        activeViewIndex_ = visibleViews.empty() ? 0 : (int)visibleViews.size() - 1;
    }

    // Draw Layout Lines and Menu
    for (int i = 0; i < height; ++i) {
        std::cout << "\033[" << (i + 2) << ";1H\033[K";
        if (i < (int)visibleViews.size()) {
            if (i == activeViewIndex_) {
                std::cout << colorActive_ << " > " << visibleViews[i]->getTitle() << " \033[0m";
            } else {
                std::cout << "   " << visibleViews[i]->getTitle();
            }
        }
        std::cout << "\033[" << (i + 2) << ";" << menuWidth << "H|";
        std::cout << "\033[" << (i + 2) << ";" << (menuWidth + contentWidth) << "H|";
    }

    // Render Middle Panel (Content)
    if (!visibleViews.empty()) {
        std::cout << "\033[2;" << (menuWidth + 2) << "H";
        visibleViews[activeViewIndex_]->render(config_, menuWidth + 2);

        // Render Right Panel (Explanation)
        std::string title = visibleViews[activeViewIndex_]->getTitle();
        if (explanations_.count(title)) {
            std::string exp = explanations_[title];
            int expCol = menuWidth + contentWidth + 2;
            std::cout << "\033[2;" << expCol << "H" << colorAccent_ << "\033[1mExplanation:\033[0m";
            
            // Simple word wrap for explanation
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
            if (line <= height) {
                std::cout << "\033[" << line << ";" << expCol << "H" << currentLine;
            }
        }
    }

    std::cout << "\033[" << (height + 2) << ";1H" << colorHeader_ << " [Arrows] Navigate | [Q] Quit \033[0m" << std::flush;
}

void TerminalUI::handleInput() {
#ifdef _WIN32
    int ch = _getch();
#else
    int ch = getchar();
#endif

    std::vector<ITerminalView*> visibleViews;
    for (auto& v : views_) {
        if (v->isVisible(config_)) {
            visibleViews.push_back(v.get());
        }
    }

    if (ch == 'q' || ch == 'Q') {
        running_ = false;
    } else if (ch == '\033' || ch == 224) { // ESC or arrow prefix on Windows
#ifdef _WIN32
        int next = (ch == 224) ? _getch() : _getch();
        if (ch == 224) { // Arrow keys
             switch(next) {
                case 72: if (activeViewIndex_ > 0) activeViewIndex_--; break; // Up
                case 80: if (activeViewIndex_ < (int)visibleViews.size() - 1) activeViewIndex_++; break; // Down
            }
        }
#else
        int next = getchar();
        if (next == '[') {
            switch(getchar()) {
                case 'A': if (activeViewIndex_ > 0) activeViewIndex_--; break;
                case 'B': if (activeViewIndex_ < (int)visibleViews.size() - 1) activeViewIndex_++; break;
            }
        }
#endif
    } else if (!visibleViews.empty() && activeViewIndex_ < (int)visibleViews.size()) {
        visibleViews[activeViewIndex_]->handleInput(ch, config_);
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
