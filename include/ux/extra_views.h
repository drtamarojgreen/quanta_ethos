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

class DiffViewer : public ITerminalView {
public:
    std::string getTitle() const override { return "Diff Viewer"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mSide-by-Side Diff Viewer\033[0m\n";
        std::cout << "\033[2;" << startCol << "H" << "\033[1;32m+ New Version\033[0m | \033[1;31m- Old Version\033[0m\n";
        std::cout << "\033[3;" << startCol << "HLine 1: No change  | Line 1: No change\n";
        std::cout << "\033[4;" << startCol << "H" << "\033[32mLine 2: Added text \033[0m | Line 2: \n";
        std::cout << "\033[5;" << startCol << "HLine 3: No change  | " << "\033[31mLine 3: Removed text\033[0m\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class ConfigMigrationView : public ITerminalView {
public:
    std::string getTitle() const override { return "Migration"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mConfig Migration Assistant\033[0m\n";
        std::cout << "\033[2;" << startCol << "HCurrent Version: 1.0\n";
        std::cout << "\033[3;" << startCol << "HTarget Version:  1.2\n";
        std::cout << "\033[5;" << startCol << "HChanges:\n";
        std::cout << "\033[6;" << startCol << "H- ui.theme -> ui.style [RENAMED]\n";
        std::cout << "\033[7;" << startCol << "H- engine.threads [REQUIRED]\n";
        std::cout << "\033[9;" << startCol << "H[M] Start Migration | [L] Lint Config";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class SimulationView : public ITerminalView {
public:
    std::string getTitle() const override { return "Simulation"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mWhat-If Simulation\033[0m\n";
        std::cout << "\033[2;" << startCol << "HScenario: Increase Threads to 16\n";
        std::cout << "\033[4;" << startCol << "HImpact Analysis:\n";
        std::cout << "\033[5;" << startCol << "H- Latency:   -15% (Predicted)\n";
        std::cout << "\033[6;" << startCol << "H- CPU Load:  +40% (Predicted)\n";
        std::cout << "\033[7;" << startCol << "H- Risk:      LOW\n";
        std::cout << "\033[9;" << startCol << "H[E] Execute Scenario | [X] Cancel";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class MaintenanceView : public ITerminalView {
public:
    std::string getTitle() const override { return "Maintenance"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mMaintenance Scheduler\033[0m\n";
        std::cout << "\033[2;" << startCol << "HNext Window: 2024-05-25 02:00 UTC\n";
        std::cout << "\033[3;" << startCol << "HDuration:    2h\n";
        std::cout << "\033[4;" << startCol << "HType:        SYSTEM_UPGRADE\n";
        std::cout << "\033[6;" << startCol << "H[S] Reschedule | [C] Cancel Window";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class AnomalyDetectorView : public ITerminalView {
public:
    std::string getTitle() const override { return "Anomalies"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mMetric Anomaly Detection\033[0m\n";
        std::cout << "\033[2;" << startCol << "H" << "\033[1;31m[!] SPIKE: Latency reached 800ms\033[0m\n";
        std::cout << "\033[3;" << startCol << "H[ ] Steady: Throughput (10k/s)\n";
        std::cout << "\033[4;" << startCol << "H" << "\033[1;33m[?] DRIFT: Memory usage increasing\033[0m\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class EmbeddedEditorView : public ITerminalView {
public:
    std::string getTitle() const override { return "Editor"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mEmbedded Text Editor\033[0m\n";
        int line = 2;
        for (size_t i = 0; i < content_.size(); ++i) {
            std::cout << "\033[" << line++ << ";" << startCol << "H"
                      << (i == cursorLine_ ? "> " : "  ") << content_[i] << "\n";
        }
        std::cout << "\033[10;" << startCol << "H[I] Insert | [X] External | [S] Save";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 'j' && cursorLine_ < content_.size() - 1) cursorLine_++;
        else if (input == 'k' && cursorLine_ > 0) cursorLine_--;
        else if (input == 'x' || input == 'X') {
            std::system("vi /tmp/pq_edit.txt"); // Round-trip mock
        }
    }
private:
    std::vector<std::string> content_ = {"// PrismQuanta Config", "system.threads=4", "ui.theme=emerald"};
    size_t cursorLine_ = 0;
};

class WorkflowReviewView : public ITerminalView {
public:
    std::string getTitle() const override { return "Workflow"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mDraft Review & Approval\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- Draft #123: CFG_UPDATE\n";
        std::cout << "\033[3;" << startCol << "H- Author: Precision Power\n";
        std::cout << "\033[4;" << startCol << "H- Status: PENDING_REVIEW\n";
        std::cout << "\033[6;" << startCol << "H[A] Approve | [R] Reject | [D] Diff";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class LocalizationView : public ITerminalView {
public:
    std::string getTitle() const override { return "Localization"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mLocalization (i18n)\033[0m\n";
        std::cout << "\033[2;" << startCol << "HCurrent Locale: " << config.get("system.locale", "en_US") << "\n";
        std::cout << "\033[4;" << startCol << "H- [E] English\n";
        std::cout << "\033[5;" << startCol << "H- [S] Spanish\n";
        std::cout << "\033[6;" << startCol << "H- [F] French\n";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 'e' || input == 'E') config.set("system.locale", "en_US");
        else if (input == 's' || input == 'S') config.set("system.locale", "es_ES");
        else if (input == 'f' || input == 'F') config.set("system.locale", "fr_FR");
    }
};

class AccessibilityView : public ITerminalView {
public:
    std::string getTitle() const override { return "A11y"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mAccessibility Settings\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- Focus Ring:    [" << config.get("ui.a11y.focus", "OFF") << "]\n";
        std::cout << "\033[3;" << startCol << "H- Screen Reader: [" << config.get("ui.a11y.reader", "OFF") << "]\n";
        std::cout << "\033[4;" << startCol << "H- Audible Bell:  [" << config.get("ui.a11y.bell", "ON") << "]\n";
        std::cout << "\033[6;" << startCol << "H[F/R/B] Toggle Options";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 'f' || input == 'F') config.set("ui.a11y.focus", config.get("ui.a11y.focus") == "ON" ? "OFF" : "ON");
        else if (input == 'r' || input == 'R') config.set("ui.a11y.reader", config.get("ui.a11y.reader") == "ON" ? "OFF" : "ON");
        else if (input == 'b' || input == 'B') config.set("ui.a11y.bell", config.get("ui.a11y.bell") == "ON" ? "OFF" : "ON");
    }
};

class OnboardingTutorialView : public ITerminalView {
public:
    std::string getTitle() const override { return "Onboarding"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mWelcome to PrismQuanta Glia\033[0m\n";
        std::cout << "\033[2;" << startCol << "HStep 1: Use ':' to enter commands.\n";
        std::cout << "\033[3;" << startCol << "HStep 2: Try 'layout_grid' or 'layout_focus'.\n";
        std::cout << "\033[4;" << startCol << "HStep 3: Press '?' for the shortcut cheat sheet.\n";
        std::cout << "\033[6;" << startCol << "H[Space] Finish Tutorial";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class RecentScreensView : public ITerminalView {
public:
    RecentScreensView(TerminalUI& ui) : ui_(ui) {}
    std::string getTitle() const override { return "Recent Screens"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mRecent & Bookmarks\033[0m\n";
        std::cout << "\033[2;" << startCol << "HBookmarks:\n";
        for (int b : ui_.getBookmarks()) std::cout << "\033[3;" << startCol << "H- Screen #" << b << "\n";
        std::cout << "\033[5;" << startCol << "HRecent History:\n";
        int line = 6;
        for (int r : ui_.getRecentScreens()) std::cout << "\033[" << line++ << ";" << startCol << "H- Screen #" << r << "\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
private:
    TerminalUI& ui_;
};

class NotificationHistoryView : public ITerminalView {
public:
    NotificationHistoryView(TerminalUI& ui) : ui_(ui) {}
    std::string getTitle() const override { return "Notes History"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mNotification History\033[0m\n";
        int line = 2;
        for (const auto& n : ui_.getNotificationHistory()) {
            std::cout << "\033[" << line++ << ";" << startCol << "H- " << n.message << "\n";
            if (line > 18) break;
        }
    }

    void handleInput(int input, ConfigEngine& config) override {}
private:
    TerminalUI& ui_;
};

class TreeExplorerView : public ITerminalView {
public:
    std::string getTitle() const override { return "Tree Explorer"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mConfig Tree Explorer\033[0m\n";
        std::cout << "\033[2;" << startCol << "H" << (expanded_ ? "[-] root" : "[+] root") << "\n";
        if (expanded_) {
            std::cout << "\033[3;" << startCol << "H  |-- ui\n";
            std::cout << "\033[4;" << startCol << "H  |   |-- theme: " << config.get("ui.theme", "default") << "\n";
            std::cout << "\033[5;" << startCol << "H  |-- system\n";
        }
        std::cout << "\033[10;" << startCol << "H[Space] Toggle Node";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == ' ') expanded_ = !expanded_;
    }
private:
    bool expanded_ = true;
};

class ObservabilityView : public ITerminalView {
public:
    std::string getTitle() const override { return "Observability"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mSystem Observability Charts\033[0m\n";
        std::cout << "\033[2;" << startCol << "HLatency:  ▃▅▇▉▇▆▅▄▃ \n";
        std::cout << "\033[3;" << startCol << "HThroughput: ▂▃▄▅▆▇▉▇▆▅ \n";
        std::cout << "\033[5;" << startCol << "HCPU Load: [||||||    ] 60%\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class AlertCenterView : public ITerminalView {
public:
    std::string getTitle() const override { return "Alerts"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mActive Alerts\033[0m\n";
        if (alerts_.empty()) std::cout << "\033[2;" << startCol << "HNo active alerts.\n";
        else {
            int line = 2;
            for (const auto& a : alerts_) std::cout << "\033[" << line++ << ";" << startCol << "H[!] " << a << "\n";
        }
        std::cout << "\033[10;" << startCol << "H[A] Ack All | [S] Snooze";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 'a' || input == 'A') alerts_.clear();
    }
private:
    std::vector<std::string> alerts_ = {"High Latency (150ms)", "Low Disk Space (5%)"};
};

class VirtualListView : public ITerminalView {
public:
    std::string getTitle() const override { return "Big List"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mVirtualized List (Page " << (offset_/5 + 1) << ")\033[0m\n";
        for (int i = 0; i < 5; ++i) {
            std::cout << "\033[" << (i+2) << ";" << startCol << "HItem #" << (offset_ + i + 1) << "\n";
        }
        std::cout << "\033[10;" << startCol << "H[N] Next Page | [P] Prev Page";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 'n' || input == 'N') offset_ += 5;
        else if (input == 'p' || input == 'P' && offset_ >= 5) offset_ -= 5;
    }
private:
    int offset_ = 0;
};

class AuditTrailView : public ITerminalView {
public:
    std::string getTitle() const override { return "Audit Trail"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mSecurity Audit Log\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- user:admin | action:LOGIN\n";
        std::cout << "\033[3;" << startCol << "H- user:admin | action:CFG_EDIT\n";
        std::cout << "\033[4;" << startCol << "H- user:admin | action:EXPORT\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class ProfileManagerView : public ITerminalView {
public:
    std::string getTitle() const override { return "Profiles"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mAccount Profiles\033[0m\n";
        std::cout << "\033[2;" << startCol << "HActive: Precision Power (Admin)\n";
        std::cout << "\033[3;" << startCol << "H- Org: PrismQuanta Dev\n";
        std::cout << "\033[4;" << startCol << "H- Status: Authenticated (SSO)\n";
        std::cout << "\033[6;" << startCol << "H[S] Switch Profile | [L] Logout";
    }

    void handleInput(int input, ConfigEngine& config) override {}
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
            {"Core", "Active", "12ms", "Group:A"},
            {"Ethos", "Active", "45ms", "Group:B"},
            {"Model", "Standby", "0ms", "Group:A"},
            {"IO", "Busy", "89ms", "Group:B"},
            {"Security", "Active", "5ms", "Group:A"}
        };
    }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mResource Monitor Grid\033[0m\n";
        std::cout << "\033[2;" << startCol << "H" << "\033[4mID  | Name       | Status   | Latency | Tag\033[0m\n";

        int line = 3;
        for (size_t i = 0; i < data_.size(); ++i) {
            bool sel = std::find(selected_.begin(), selected_.end(), i) != selected_.end();
            std::cout << "\033[" << line++ << ";" << startCol << "H"
                      << (sel ? "\033[1;33m[*] " : "[ ] ")
                      << std::left << std::setw(10) << data_[i][0] << " | "
                      << std::left << std::setw(8) << data_[i][1] << " | "
                      << std::left << std::setw(7) << data_[i][2] << " | " << data_[i][3] << "\033[0m\n";
        }
        std::cout << "\033[10;" << startCol << "H[S] Sort | [Space] Select | [B] Bulk Action | [Q] Saved Queries";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (input == 's' || input == 'S') {
            std::sort(data_.begin(), data_.end(), [](const auto& a, const auto& b) {
                return std::stoi(a[2]) > std::stoi(b[2]);
            });
        } else if (input == ' ') {
            // Toggle selection for first row for demo
            if (selected_.empty()) selected_.push_back(0); else selected_.clear();
        }
    }

private:
    std::vector<std::vector<std::string>> data_;
    std::vector<int> selected_;
};

class SavedQueriesView : public ITerminalView {
public:
    std::string getTitle() const override { return "Queries"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mSaved Query Library\033[0m\n";
        std::cout << "\033[2;" << startCol << "H1. Active Resources (latency > 10ms)\n";
        std::cout << "\033[3;" << startCol << "H2. Group A Entities\n";
        std::cout << "\033[4;" << startCol << "H3. Critical Status Overrides\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
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

class StartupProfilerView : public ITerminalView {
public:
    std::string getTitle() const override { return "Profiler"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mStartup Performance Profiler\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- Config Load:   12ms\n";
        std::cout << "\033[3;" << startCol << "H- Plugin Init:   45ms\n";
        std::cout << "\033[4;" << startCol << "H- UI Boot:       89ms\n";
        std::cout << "\033[5;" << startCol << "H- Async Pipeline: 5ms\n";
        std::cout << "\033[7;" << startCol << "HTotal Boot:     151ms\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};

class TermDiagnosticsView : public ITerminalView {
public:
    std::string getTitle() const override { return "Term Diag"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mTerminal Capabilities\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- TERM: " << getenv("TERM") << "\n";
        std::cout << "\033[3;" << startCol << "H- 256 Color: [YES]\n";
        std::cout << "\033[4;" << startCol << "H- Mouse Track: [ENABLED]\n";
        std::cout << "\033[5;" << startCol << "H- Unicode: " << config.get("ui.unicode", "true") << "\n";
    }

    void handleInput(int input, ConfigEngine& config) override {}
};
