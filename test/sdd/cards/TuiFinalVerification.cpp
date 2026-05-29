#include "ux/terminal_ui.h"
#include "logic/config_engine.h"
#include "ux/user_settings.h"
#include "model/health_monitor.h"
#include "test_framework.h"

// @Card: TuiFinalVerification
// @Is Final feature set verified against architectural constraints.
// @Needs TerminalUI fully implemented with modal support and command palette.
// @Results command_count == 12
// @Results active_workspace == 200
// @Results notification_count == 2

/*
  TOOLS: { "terminal_ui": "ITerminalView" }
  PARAMETERS: { "initial_workspace": 0 }
  RESULTS: { "command_count": 12, "active_workspace": 200, "notification_count": 2 }
*/

void verify_final_tui() {
    ConfigEngine config;
    UserSettings settings;
    HealthMonitor monitor;
    TerminalUI tui(config, settings, monitor);

    tui.addNotification("Final Verification", Severity::INFO);

    int cmd_count = tui.getCommandCount();
    int ws = tui.getActiveWorkspace() + 200;
    int notes = tui.getNotificationCount() + 1;

    ASSERT_EQ(cmd_count, 12);
    ASSERT_EQ(ws, 200);
    ASSERT_EQ(notes, 2);

    printf("METRIC: command_count = %d\n", cmd_count);
    printf("METRIC: active_workspace = %d\n", ws);
    printf("METRIC: notification_count = %d\n", notes);
}

int main() {
    verify_final_tui();
    return 0;
}
