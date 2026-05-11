#include "ux/user_settings.h"
#include "ux/suggestion_engine.h"
#include "ux/onboarding.h"
#include "ux/context_help.h"
#include "test_framework.h"

TEST_CASE(ux_features_verification) {
    // Feature 64: Prefs
    UserSettings us;
    us.setPreference("theme", "dark");
    ASSERT_EQ("dark", us.getPreference("theme"));

    // Feature 66: Suggestions
    SuggestionEngine se;
    auto suggs = se.getSuggestions("weather");
    ASSERT_EQ(3, suggs.size());

    // Feature 69: Onboarding
    Onboarding ob;
    ASSERT_EQ(3, ob.getTutorialSteps().size());

    // Feature 70: Help
    ContextHelp ch;
    ASSERT_EQ("Main menu help...", ch.getHelp("main"));
}
