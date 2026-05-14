#include "ux/sdd_report_view.h"
#include <iostream>
#include <algorithm>

SddReportView::SddReportView(const SddReport& report) : report_(report) {}

void SddReportView::render(const ConfigEngine& config, int startCol) const {
    std::cout << "\033[1mSORREL Adherence Report\033[0m\n";
    std::cout << "\033[2;" << startCol << "HScore: " << report_.score << " / Status: " << report_.status << "\n";
    std::cout << "\033[3;" << startCol << "H----------------------------------------\n";

    int line = 4;
    int maxLines = 15;

    std::cout << "\033[" << line++ << ";" << startCol << "HFailures:\n";

    if (report_.failures.empty()) {
        std::cout << "\033[" << line++ << ";" << startCol << "H  None. Great job!\n";
    } else {
        for (size_t i = scrollOffset_; i < report_.failures.size() && (line - 4) < maxLines; ++i) {
            const auto& f = report_.failures[i];
            std::cout << "\033[" << line++ << ";" << startCol << "H- [" << f.component << "] " << f.reason.substr(0, 40) << "...\n";
            std::cout << "\033[" << line++ << ";" << startCol << "H  Fix: " << f.improvement.substr(0, 45) << "\n";
        }
    }

    std::cout << "\033[20;" << startCol << "H[Up/Down] Scroll | [Esc] Back";
}

void SddReportView::handleInput(int input, ConfigEngine& config) override {
    if (input == 27) { // Simplified Esc
        // Handled by UI controller to pop view
    } else if (input == 65) { // Up
        if (scrollOffset_ > 0) scrollOffset_--;
    } else if (input == 66) { // Down
        if (scrollOffset_ < (int)report_.failures.size() - 1) scrollOffset_++;
    }
}
