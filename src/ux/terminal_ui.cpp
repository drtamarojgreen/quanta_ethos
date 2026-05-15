#include "ux/terminal_ui.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

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

void TerminalUI::drawLayout() {
    // Clear screen
    std::cout << "\033[2J\033[H";

    // Draw header
    std::cout << "\033[1;37;44m PrismQuanta System Configuration \033[0m\n";

    int height = 20;
    int width = 80;
    int menuWidth = 16;
    int contentWidth = 34;
    int explanationWidth = 30;

    // Draw Layout Lines and Menu
    for (int i = 0; i < height; ++i) {
        std::cout << "\033[" << (i + 2) << ";1H\033[K";
        if (i < (int)views_.size()) {
            if (i == activeViewIndex_) {
                std::cout << "\033[1;30;47m > " << views_[i]->getTitle() << " \033[0m";
            } else {
                std::cout << "   " << views_[i]->getTitle();
            }
        }
        std::cout << "\033[" << (i + 2) << ";" << menuWidth << "H|";
        std::cout << "\033[" << (i + 2) << ";" << (menuWidth + contentWidth) << "H|";
    }

    // Render Middle Panel (Content)
    if (!views_.empty() && activeViewIndex_ < (int)views_.size()) {
        std::cout << "\033[2;" << (menuWidth + 2) << "H";
        views_[activeViewIndex_]->render(config_, menuWidth + 2);

        // Render Right Panel (Explanation)
        std::string title = views_[activeViewIndex_]->getTitle();
        if (explanations_.count(title)) {
            std::string exp = explanations_[title];
            int expCol = menuWidth + contentWidth + 2;
            std::cout << "\033[2;" << expCol << "H\033[1mExplanation:\033[0m";
            
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

    std::cout << "\033[" << (height + 2) << ";1H\033[1;37;44m [Arrows] Navigate | [Q] Quit \033[0m" << std::flush;
}

void TerminalUI::handleInput() {
#ifdef _WIN32
    int ch = _getch();
#else
    int ch = getchar();
#endif

    if (ch == 'q' || ch == 'Q') {
        running_ = false;
    } else if (ch == '\033' || ch == 224) { // ESC or arrow prefix on Windows
#ifdef _WIN32
        int next = (ch == 224) ? _getch() : _getch();
        if (ch == 224) { // Arrow keys
             switch(next) {
                case 72: if (activeViewIndex_ > 0) activeViewIndex_--; break; // Up
                case 80: if (activeViewIndex_ < (int)views_.size() - 1) activeViewIndex_++; break; // Down
            }
        }
#else
        int next = getchar();
        if (next == '[') {
            switch(getchar()) {
                case 'A': if (activeViewIndex_ > 0) activeViewIndex_--; break;
                case 'B': if (activeViewIndex_ < (int)views_.size() - 1) activeViewIndex_++; break;
            }
        }
#endif
    } else if (!views_.empty()) {
        views_[activeViewIndex_]->handleInput(ch, config_);
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
