#include <iostream>
#include <string>
#include <memory>

#include "core_engine.h"
#include "ux/terminal_ui.h"
#include "ux/subsystem_views.h"
#include "ux/sdd_checker_view.h"
#include "ux/extra_views.h"
#include "ux/user_settings.h"
#include "model/health_monitor.h"
#include "logic/config_engine.h"
#include "logic/async_engine.h"

int main(int argc, char* argv[]) {
    ConfigEngine config;
    config.load("prismquanta.conf");
    config.load(".quanta");

    // Check for CLI flag
    if (argc > 1 && std::string(argv[1]) == "--cli") {
        std::string prompt = "Hello from PrismQuanta";
        if (argc > 2) {
            prompt = argv[2];
        }
        CoreEngine core_engine;
        std::cout << core_engine.generate_response(prompt) << std::endl;
        return 0;
    }

    // Launch Terminal UI by default
    UserSettings settings;
    HealthMonitor monitor;
    AsyncPipeline pipeline(4);
    TerminalUI ui(config, settings, monitor, pipeline);
    ui.addView(std::make_unique<FileMenuView>());
    ui.addView(std::make_unique<ViewSettingsMenuView>());
    ui.addView(std::make_unique<ExportMenuView>());
    ui.addView(std::make_unique<CoreEngineView>());
    ui.addView(std::make_unique<EthosView>());
    ui.addView(std::make_unique<ModelView>());
    ui.addView(std::make_unique<SddCheckerView>());
    ui.addView(std::make_unique<SorrelMenuView>());
    ui.addView(std::make_unique<ProgressView>(pipeline));
    ui.addView(std::make_unique<PluginsMenuView>());
    ui.addView(std::make_unique<DataAnalysisMenuView>());
    ui.addView(std::make_unique<DiffViewer>());
    ui.addView(std::make_unique<EmbeddedEditorView>());
    ui.addView(std::make_unique<WorkflowReviewView>());
    ui.addView(std::make_unique<SimulationView>());
    ui.addView(std::make_unique<MaintenanceView>());
    ui.addView(std::make_unique<AnomalyDetectorView>());
    ui.addView(std::make_unique<TreeExplorerView>());
    ui.addView(std::make_unique<ObservabilityView>());
    ui.addView(std::make_unique<AlertCenterView>());
    ui.addView(std::make_unique<VirtualListView>());
    ui.addView(std::make_unique<AuditTrailView>());
    ui.addView(std::make_unique<ProfileManagerView>());
    ui.addView(std::make_unique<DataGridView>());
    ui.addView(std::make_unique<SavedQueriesView>());
    ui.addView(std::make_unique<PluginMarketplaceView>());
    ui.addView(std::make_unique<ChangelogView>());
    ui.addView(std::make_unique<IncidentTimelineView>(monitor));
    ui.addView(std::make_unique<UserAnalyticsView>());
    ui.addView(std::make_unique<ConfigMigrationView>());
    ui.addView(std::make_unique<OnboardingTutorialView>());
    ui.addView(std::make_unique<RecentScreensView>(ui));
    ui.addView(std::make_unique<NotificationHistoryView>(ui));
    ui.addView(std::make_unique<LocalizationView>());
    ui.addView(std::make_unique<AccessibilityView>());
    ui.addView(std::make_unique<StartupProfilerView>());
    ui.addView(std::make_unique<TermDiagnosticsView>());

    config.set("system.view_count", "37");

    ui.run();

    return 0;
}
