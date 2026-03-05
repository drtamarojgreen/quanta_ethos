#include <string>

#include "model_backend.h"
#include "test_framework.h"

TEST_CASE(run_model_returns_stitched_output) {
    ModelBackend backend;
    ASSERT_EQ("Model output for: hello", backend.run_model("hello"));
}

TEST_CASE(run_model_preserves_input_text) {
    ModelBackend backend;
    const std::string input = "safety constraints";
    const std::string output = backend.run_model(input);

    ASSERT_TRUE(output.find(input) != std::string::npos);
}
