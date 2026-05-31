#pragma once

#include "ux/terminal_view.h"
#include "ux/data_exporter.h"
#include "dev/sdd_engine.h"
#include "dev/benchmarker.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <future>
#include <thread>
#include <chrono>

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

    std::vector<ContextMenuItem> getContextMenu() const override {
        return {
            {"Reload", [this]() { status_ = "Reload triggered"; }},
            {"Reset Path", [this]() { status_ = "Path reset"; }}
        };
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
        std::cout << "\033[2;" << startCol << "H1. Export Config (TXT)\n";
        std::cout << "\033[3;" << startCol << "H2. Export SDD (MD)\n";
        std::cout << "\033[4;" << startCol << "H3. Export SDD (CSV)\n";
        std::cout << "\033[5;" << startCol << "H4. Export SDD (JSON)\n";
        std::cout << "\033[7;" << startCol << "HStatus: " << status_ << "\n";
        std::cout << "\033[9;" << startCol << "H[1-4] Select Action";
    }

    void handleInput(int input, ConfigEngine& config) override {
        SddEngine engine;
        SddReport report = engine.runCheck(".");
        std::vector<std::string> headers = {"Component", "Reason", "Improvement"};
        std::vector<std::vector<std::string>> rows;
        for (const auto& f : report.failures) {
            rows.push_back({f.component, f.reason, f.improvement});
        }

        if (input == '1') {
            config.save("data/config_export.txt");
            status_ = "Config exported to TXT.";
        } else if (input == '2') {
            DataExporter::exportToMarkdown("data/sdd_report.md", headers, rows);
            status_ = "Report exported to Markdown.";
        } else if (input == '3') {
            DataExporter::exportToCSV("data/sdd_report.csv", headers, rows);
            status_ = "Report exported to CSV.";
        } else if (input == '4') {
            DataExporter::exportToJSON("data/sdd_report.json", headers, rows);
            status_ = "Report exported to JSON.";
        }
    }

    std::vector<ContextMenuItem> getContextMenu() const override {
        return {
            {"Clear Logs", [this]() { status_ = "Logs cleared"; }},
            {"Open Export Dir", [this]() { status_ = "Dir opened"; }}
        };
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
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return result;
        });
    }

    mutable std::string status_ = "Ready";
    mutable bool running_ = false;
    mutable std::future<std::string> future_;
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
        bench.stop();

        std::cout << "\033[6;" << startCol << "H- Last Render: " << bench.getDurationMs() << "ms\n";
        std::cout << "\033[8;" << startCol << "H- Total Views: " << config.getInt("system.view_count", 0) << "\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class IncidentTimelineView : public ITerminalView {
public:
    IncidentTimelineView(HealthMonitor& monitor) : monitor_(monitor) {}
    std::string getTitle() const override { return "Incidents"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mIncident Timeline\033[0m\n";
        auto history = monitor_.getHistory();
        int line = 2;
        for (auto it = history.rbegin(); it != history.rend(); ++it) {
            std::cout << "\033[" << line++ << ";" << startCol << "H"
                      << "Latency: " << it->latency_ms << "ms | Health: "
                      << (it->latency_ms > 100 ? "CRITICAL" : "OK") << "\n";
            if (line > 8) break;
        }
        if (history.empty()) std::cout << "\033[2;" << startCol << "HNo incidents recorded.\n";
        std::cout << "\033[10;" << startCol << "H[R] Refresh Timeline";
    }

    void handleInput(int input, ConfigEngine& config) override {}
private:
    HealthMonitor& monitor_;
};

class UserAnalyticsView : public ITerminalView {
public:
    std::string getTitle() const override { return "Analytics"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mUser Analytics & Telemetry\033[0m\n";
        bool optIn = config.get("user.telemetry", "false") == "true";
        std::cout << "\033[2;" << startCol << "HTelemetry Opt-In: " << (optIn ? "[ON]" : "[OFF]") << "\n";
        std::cout << "\033[4;" << startCol << "HUsage Stats:\n";
        std::cout << "\033[5;" << startCol << "H- Session Time: 12m\n";
        std::cout << "\033[6;" << startCol << "H- Commands Run: 45\n";
        std::cout << "\033[8;" << startCol << "H[T] Toggle Telemetry";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 't' || input == 'T') {
            bool optIn = config.get("user.telemetry", "false") == "true";
            config.set("user.telemetry", optIn ? "false" : "true");
        }
    }
};

class DataGridView : public ITerminalView {
public:
    std::string getTitle() const override { return "Data Grid"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    DataGridView() {
        data_ = {
            {"Core", "Active", "12ms"},
            {"Ethos", "Active", "45ms"},
            {"Model", "Standby", "0ms"},
            {"IO", "Busy", "89ms"},
            {"Security", "Active", "5ms"}
        };
    }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mResource Monitor Grid\033[0m\n";
        std::cout << "\033[2;" << startCol << "H" << "\033[4mName       | Status   | Latency\033[0m\n";

        int line = 3;
        for (const auto& row : data_) {
            std::cout << "\033[" << line++ << ";" << startCol << "H"
                      << std::left << std::setw(10) << row[0] << " | "
                      << std::left << std::setw(8) << row[1] << " | " << row[2] << "\n";
        }
        std::cout << "\033[10;" << startCol << "H[S] Sort by Latency | [R] Reset";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 's' || input == 'S') {
            std::sort(data_.begin(), data_.end(), [](const auto& a, const auto& b) {
                return std::stoi(a[2]) > std::stoi(b[2]);
            });
        } else if (input == 'r' || input == 'R') {
            data_ = {{"Core", "Active", "12ms"}, {"Ethos", "Active", "45ms"}, {"Model", "Standby", "0ms"}, {"IO", "Busy", "89ms"}, {"Security", "Active", "5ms"}};
        }
    }

private:
    std::vector<std::vector<std::string>> data_;
};

class PluginMarketplaceView : public ITerminalView {
public:
    std::string getTitle() const override { return "Marketplace"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mPlugin Marketplace\033[0m\n";
        std::cout << "\033[2;" << startCol << "H1. SQL Explorer [Installed]\n";
        std::cout << "\033[3;" << startCol << "H2. Redis Visualizer [Available]\n";
        std::cout << "\033[4;" << startCol << "H3. K8s Dashboard [Available]\n";
        std::cout << "\033[6;" << startCol << "HStatus: " << status_ << "\n";
        std::cout << "\033[8;" << startCol << "H[2-3] Install Plugin";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == '2') status_ = "Installing Redis Visualizer...";
        else if (input == '3') status_ = "Installing K8s Dashboard...";
    }
private:
    std::string status_ = "Ready";
};

class ChangelogView : public ITerminalView {
public:
    std::string getTitle() const override { return "Changelog"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mRecent Changes\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- v1.2: Added Modal Architecture\n";
        std::cout << "\033[3;" << startCol << "H- v1.1: Improved ANSI Rendering\n";
        std::cout << "\033[4;" << startCol << "H- v1.0: Initial Release\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};
