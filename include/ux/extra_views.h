#pragma once

#include "ux/terminal_view.h"
#include "dev/sdd_engine.h"
#include "dev/benchmarker.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <future>

namespace fs = std::filesystem;

class FileMenuView : public ITerminalView {
public:
    std::string getTitle() const override { return "File"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mFile Management\033[0m\n";
        std::cout << "\033[2;" << startCol << "H1. Load Config: " << configPath_ << "\n";
        std::cout << "\033[3;" << startCol << "H2. Save Config\n";
        std::cout << "\033[5;" << startCol << "HStatus: " << status_ << "\n";
        std::cout << "\033[7;" << startCol << "H[1-2] Select Action";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == '1') {
            config.load(configPath_);
            status_ = "Configuration loaded from " + configPath_;
        } else if (input == '2') {
            config.save(configPath_);
            status_ = "Configuration saved to " + configPath_;
        }
    }
private:
    std::string configPath_ = "prismquanta.conf";
    std::string status_ = "Ready";
};

class ExportMenuView : public ITerminalView {
public:
    std::string getTitle() const override { return "Export"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mExport Data\033[0m\n";
        std::cout << "\033[2;" << startCol << "H1. Export Config to data/config_export.txt\n";
        std::cout << "\033[3;" << startCol << "H2. Export SDD Report to data/sdd_export.txt\n";
        std::cout << "\033[5;" << startCol << "HStatus: " << status_ << "\n";
        std::cout << "\033[7;" << startCol << "H[1-2] Select Action";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == '1') {
            config.save("data/config_export.txt");
            status_ = "Config exported.";
        } else if (input == '2') {
            SddEngine engine;
            SddReport report = engine.runCheck(".");
            std::ofstream file("data/sdd_export.txt");
            file << "SDD Report Score: " << report.score << "\nStatus: " << report.status << "\n";
            status_ = "SDD Report exported.";
        }
    }
private:
    std::string status_ = "Ready";
};

class ViewSettingsMenuView : public ITerminalView {
public:
    std::string getTitle() const override { return "View Settings"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mView & Theme Settings\033[0m\n";
        std::cout << "\033[2;" << startCol << "H1. Theme: " << config.get("ui.theme", "default") << "\n";
        std::cout << "\033[4;" << startCol << "HVisibility Toggles:\n";
        std::cout << "\033[5;" << startCol << "H- [C] Core Engine: " << config.get("view.core_engine.visible", "true") << "\n";
        std::cout << "\033[6;" << startCol << "H- [E] Ethical Gov: " << config.get("view.ethos.visible", "true") << "\n";
        std::cout << "\033[7;" << startCol << "H- [M] Model Backend: " << config.get("view.model_backend.visible", "true") << "\n";
        std::cout << "\033[9;" << startCol << "H[1/C/E/M] Toggle Settings";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == '1') {
            std::string current = config.get("ui.theme", "default");
            if (current == "default") config.set("ui.theme", "emerald");
            else if (current == "emerald") config.set("ui.theme", "amber");
            else config.set("ui.theme", "default");
        } else if (input == 'c' || input == 'C') {
            config.set("view.core_engine.visible", config.get("view.core_engine.visible") == "true" ? "false" : "true");
        } else if (input == 'e' || input == 'E') {
            config.set("view.ethos.visible", config.get("view.ethos.visible") == "true" ? "false" : "true");
        } else if (input == 'm' || input == 'M') {
            config.set("view.model_backend.visible", config.get("view.model_backend.visible") == "true" ? "false" : "true");
        }
    }
};

class SorrelMenuView : public ITerminalView {
public:
    std::string getTitle() const override { return "Sorrel Ops"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mSorrel Operations\033[0m\n";
        std::cout << "\033[2;" << startCol << "H1. Run SDD Check\n";
        std::cout << "\033[3;" << startCol << "H2. Initialize SDD\n";
        std::cout << "\033[5;" << startCol << "HStatus: " << status_ << "\n";
        if (running_) {
            std::cout << "\033[6;" << startCol << "HExecuting... " << spinner_[spinnerIndex_++ % 4] << "\n";
            if (future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                status_ = future_.get();
                running_ = false;
            }
        }
        std::cout << "\033[8;" << startCol << "H[1-2] Execute";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (running_) return;
        if (input == '1') {
            startAsyncOp("test/sdd/sorrel check .");
        } else if (input == '2') {
            startAsyncOp("test/sdd/sorrel init .");
        }
    }

private:
    void startAsyncOp(const std::string& cmd) {
        running_ = true;
        future_ = std::async(std::launch::async, [cmd]() {
            std::string result = "Op Completed: " + cmd;
            // Mock execution for TUI responsiveness
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return result;
        });
    }

    std::string status_ = "Ready";
    bool running_ = false;
    std::future<std::string> future_;
    mutable int spinnerIndex_ = 0;
    const std::vector<char> spinner_ = {'|', '/', '-', '\\'};
};

class PluginsMenuView : public ITerminalView {
public:
    std::string getTitle() const override { return "Plugins"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mPlugin Manager\033[0m\n";
        std::cout << "\033[2;" << startCol << "HAvailable Plugins:\n";
        int line = 3;
        for (const auto& p : plugins_) {
            std::cout << "\033[" << line++ << ";" << startCol << "H- " << p << "\n";
        }
        if (plugins_.empty()) std::cout << "\033[" << line++ << ";" << startCol << "H(None found in plugins/)\n";
        std::cout << "\033[10;" << startCol << "H[R] Refresh List";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 'r' || input == 'R') {
            refreshPlugins();
        }
    }
private:
    void refreshPlugins() {
        plugins_.clear();
        if (fs::exists("plugins")) {
            for (const auto& entry : fs::directory_iterator("plugins")) {
                plugins_.push_back(entry.path().filename().string());
            }
        }
    }
    std::vector<std::string> plugins_;
};

class DataAnalysisMenuView : public ITerminalView {
public:
    std::string getTitle() const override { return "Data Analysis"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mSystem Data Analysis\033[0m\n";
        SddEngine engine;
        SddReport report = engine.runCheck(".");

        std::cout << "\033[3;" << startCol << "H- SDD Score: " << report.score << "\n";
        std::cout << "\033[4;" << startCol << "H- Compliance: " << report.status << "\n";

        Benchmarker bench("TUI_Render");
        bench.start();
        // Mock some analysis work
        bench.stop();

        std::cout << "\033[6;" << startCol << "H- Last Render: " << bench.getDurationMs() << "ms\n";
        std::cout << "\033[8;" << startCol << "H- Total Views: " << config.getInt("system.view_count", 0) << "\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};
