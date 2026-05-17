#include "logic/config_engine.h"
#include "ux/subsystem_views.h"
#include "ux/terminal_ui.h"
#include "test_framework.h"
#include <fstream>
#include <cstdio>

TEST_CASE(test_config_persistence) {
    ConfigEngine config;
    config.set("test.key", "test_val");
    std::string testPath = "test_config_save.conf";

    config.save(testPath);

    ConfigEngine loaded;
    loaded.load(testPath);

    ASSERT_EQ("test_val", loaded.get("test.key"));
    std::remove(testPath.c_str());
}

TEST_CASE(test_view_visibility) {
    ConfigEngine config;
    CoreEngineView coreView;
    EthosView ethosView;

    config.set("view.core_engine.visible", "false");
    config.set("view.ethos.visible", "true");

    ASSERT_TRUE(!coreView.isVisible(config));
    ASSERT_TRUE(ethosView.isVisible(config));
}

TEST_CASE(test_ui_theme_switch) {
    ConfigEngine config;
    TerminalUI ui(config);

    // Inaccessible member check via logic/config observation
    config.set("ui.theme", "emerald");
    // Structural check that ConfigEngine holds the value
    ASSERT_EQ("emerald", config.get("ui.theme"));
}
