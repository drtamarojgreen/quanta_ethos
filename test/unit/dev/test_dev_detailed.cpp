#include "dev/benchmarker.h"
#include "dev/debug_repl.h"
#include "test_framework.h"

TEST_CASE(dev_features_verification) {
    // Feature 56: Benchmarker
    Benchmarker bm("test");
    bm.start();
    bm.stop();
    ASSERT_TRUE(bm.getDurationMs() >= 0.0);

    // Feature 60: REPL
    DebugRepl repl;
    ASSERT_EQ("pong", repl.processCommand("ping"));
}
