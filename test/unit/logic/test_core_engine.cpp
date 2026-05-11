#include <string>

#include "core_engine.h"
#include "test_framework.h"

TEST_CASE(generate_response_applies_trustworthiness_wrapper) {
    CoreEngine engine;
    ASSERT_EQ(
        "Checked: Model output for: Ethical prompt for: plan a task",
        engine.generate_response("plan a task")
    );
}

TEST_CASE(generate_response_is_deterministic_for_same_prompt) {
    CoreEngine engine;
    const std::string first = engine.generate_response("consistency check");
    const std::string second = engine.generate_response("consistency check");

    ASSERT_EQ(first, second);
}
