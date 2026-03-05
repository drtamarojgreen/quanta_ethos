#include <string>

#include "core_engine.h"
#include "native_bdd.h"

SCENARIO(core_engine_generates_ethically_checked_response) {
    GIVEN("an operator prompt asking for a summary");
    STEP_USED(given_step);

    CoreEngine engine;

    WHEN("the prompt is processed by the core engine");
    STEP_USED(when_step);
    const std::string response = engine.generate_response("summarize project status");

    THEN("the response is run through ethical and trustworthiness checks");
    STEP_USED(then_step);
    THEN_EQ(
        "Checked: Model output for: Ethical prompt for: summarize project status",
        response
    );
}

SCENARIO(core_engine_handles_empty_prompt_without_crashing) {
    GIVEN("an empty user prompt");
    STEP_USED(given_step);

    CoreEngine engine;

    WHEN("the empty prompt is processed");
    STEP_USED(when_step);
    const std::string response = engine.generate_response("");

    THEN("a checked model response is still returned");
    STEP_USED(then_step);
    THEN_EQ("Checked: Model output for: Ethical prompt for: ", response);
}
