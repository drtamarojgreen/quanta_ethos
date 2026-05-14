#pragma once

#include "ux/terminal_view.h"
#include "dev/sdd_engine.h"
#include <vector>
#include <string>

class SddReportView : public ITerminalView {
public:
    SddReportView(const SddReport& report);
    std::string getTitle() const override { return "Sorrel Report"; }
    void render(const ConfigEngine& config, int startCol) const override;
    void handleInput(int input, ConfigEngine& config) override;

private:
    SddReport report_;
    mutable int scrollOffset_ = 0;
};
