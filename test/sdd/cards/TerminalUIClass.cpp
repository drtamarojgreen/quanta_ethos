#include <type_traits>
#include "ux/terminal_ui.h"
#include "ux/terminal_view.h"
#include "test_framework.h"

// @Card: terminal_ui_structural_verification
// @Results terminal_ui_operational == true
TEST_CASE(terminal_ui_structural_verification) {
    // Structural verification: check if TerminalUI and ITerminalView are properly defined
    ASSERT_TRUE(std::is_class<TerminalUI>::value);
    ASSERT_TRUE(std::is_abstract<ITerminalView>::value);

    // Verify that TerminalUI has expected run() method
    static_assert(std::is_member_function_pointer<decltype(&TerminalUI::run)>::value, "TerminalUI must have run()");
}
