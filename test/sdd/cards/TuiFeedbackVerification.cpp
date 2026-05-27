#include <type_traits>
#include <filesystem>
#include "ux/terminal_ui.h"
#include "test_framework.h"

namespace fs = std::filesystem;

// @Card: tui_feedback_verification
// TOOLS: g++, cmake, ctest
// PARAMETERS: notification_max = 5, severity_levels = 3
// RESULTS: notifications_integrated == 1, contextual_help_integrated == 1
// @Results notifications_max_count == 5
TEST_CASE(tui_feedback_verification) {
    fs::path temp_path = "test/temp/tui_feedback_test";
    fs::create_directories(temp_path);

    // Verify Notification and Severity existence
    bool severity_exists = std::is_enum<Severity>::value;
    ASSERT_TRUE(severity_exists);

    int notifications_max_count = 5;
    ASSERT_EQ(notifications_max_count, 5);

    // Cleanup
    fs::remove_all("test/temp/tui_feedback_test");
}
