#include <string>

#include "ethics_logic.h"
#include "test_framework.h"

TEST_CASE(makeEthicalPrompt_prefixes_user_input) {
    ASSERT_EQ("Ethical prompt for: explain the weather", makeEthicalPrompt("explain the weather"));
}

TEST_CASE(applyTrustworthinessChecks_wraps_model_output) {
    ASSERT_EQ("Checked: candidate response", applyTrustworthinessChecks("candidate response"));
}

TEST_CASE(runSelfReview_returns_notes_for_output) {
    ASSERT_EQ("Self-review notes for: draft", runSelfReview("draft"));
}

TEST_CASE(computeTrustScore_returns_default_value) {
    ASSERT_NEAR(0.95f, computeTrustScore("note"), 1e-6);
}
