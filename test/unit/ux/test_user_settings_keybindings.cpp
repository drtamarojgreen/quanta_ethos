#include "ux/user_settings.h"
#include "test_framework.h"

TEST_CASE(UserSettings_Keybindings) {
    UserSettings settings;

    // Check default
    ASSERT_EQ(settings.getKeybinding("nav_up"), 'k');

    // Remap
    settings.setKeybinding("nav_up", 'w');
    ASSERT_EQ(settings.getKeybinding("nav_up"), 'w');

    // Non-existent
    ASSERT_EQ(settings.getKeybinding("missing"), -1);
}
