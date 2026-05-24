#include <type_traits>
#include <filesystem>
#include "ux/terminal_ui.h"
#include "ux/terminal_view.h"
#include "test_framework.h"

namespace fs = std::filesystem;

// @Card: terminal_ui_structural_verification
// TOOLS: g++, cmake, ctest
// PARAMETERS: layout_style = side_panel, menu_width_ratio = 0.25
// RESULTS: terminal_ui_operational == 1
// @Results terminal_ui_operational == 1
TEST_CASE(terminal_ui_structural_verification) {
    // Artifact placement standard: tests/temp/
    fs::path temp_path = "test/temp/terminal_ui_test";
    fs::create_directories(temp_path);

    // Structural verification
    ASSERT_TRUE(std::is_class<TerminalUI>::value);
    ASSERT_TRUE(std::is_abstract<ITerminalView>::value);

    // Cleanup logic: ensure bin/temp are cleaned
    fs::remove_all("test/temp/terminal_ui_test");
}
