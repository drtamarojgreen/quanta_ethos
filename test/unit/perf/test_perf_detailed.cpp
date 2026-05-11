#include "perf/computation_cache.h"
#include "perf/allocation_tracker.h"
#include "perf/numa_strategy.h"
#include "test_framework.h"

TEST_CASE(perf_features_verification) {
    // Feature 86: Comp Cache
    ComputationCache cc;
    cc.store("calc1", 42.0);
    double res;
    ASSERT_TRUE(cc.tryGet("calc1", res));
    ASSERT_EQ(42.0, res);

    // Feature 88: Alloc Tracker
    AllocationTracker at;
    at.recordAllocation(1024);
    ASSERT_EQ(1024, at.getCurrentUsage());
    at.recordDeallocation(512);
    ASSERT_EQ(512, at.getCurrentUsage());
}
