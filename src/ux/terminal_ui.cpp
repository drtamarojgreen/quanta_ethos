#include "ux/terminal_ui.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

TerminalUI::TerminalUI(ConfigEngine& config) : config_(config) {}

void TerminalUI::addView(std::unique_ptr<ITerminalView> view) {
    views_.push_back(std::move(view));
}

void TerminalUI::drawLayout() {
    // Clear screen
    std::cout << "\033[2J\033[H";

    // Draw header
    std::cout << "\033[1;37;44m PrismQuanta System Configuration \033[0m\n";

    // Draw Split Screen
    int height = 20;
    int width = 80;
    int menuWidth = width / 4;

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
    }

    if (!views_.empty() && activeViewIndex_ < (int)views_.size()) {
        std::cout << "\033[2;" << (menuWidth + 2) << "H";
        views_[activeViewIndex_]->render(config_, menuWidth + 2);
    }

    std::cout << "\033[" << (height + 2) << ";1H\033[1;37;44m [Arrows] Navigate | [Q] Quit \033[0m" << std::flush;
}

void TerminalUI::handleInput() {
    int ch = getchar();
    if (ch == 'q' || ch == 'Q') {
        running_ = false;
    } else if (ch == '\033') {
        int next = getchar();
        if (next == '[') {
            switch(getchar()) {
                case 'A': if (activeViewIndex_ > 0) activeViewIndex_--; break;
                case 'B': if (activeViewIndex_ < (int)views_.size() - 1) activeViewIndex_++; break;
            }
        }
    } else if (!views_.empty()) {
        views_[activeViewIndex_]->handleInput(ch, config_);
    }
}

void TerminalUI::run() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Hide cursor
    std::cout << "\033[?25l";

    while (running_) {
        drawLayout();
        handleInput();
    }

    // Show cursor
    std::cout << "\033[?25h";

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
