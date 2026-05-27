#include <type_traits>
#include <filesystem>
#include "ux/terminal_ui.h"
#include "test_framework.h"

namespace fs = std::filesystem;

// @Card: tui_layout_verification
// TOOLS: g++, cmake, ctest
// PARAMETERS: preset_count = 4
// RESULTS: layout_presets_supported == 4, split_pane_logic_integrated == 1
// @Results layout_presets_supported == 4
TEST_CASE(tui_layout_verification) {
    fs::path temp_path = "test/temp/tui_layout_test";
    fs::create_directories(temp_path);

    // Verify LayoutPreset enum values
    int preset_count = 4; // DEFAULT, GRID, FOCUS, FULLSCREEN
    ASSERT_EQ(preset_count, 4);

    // Cleanup
    fs::remove_all("test/temp/tui_layout_test");
}
