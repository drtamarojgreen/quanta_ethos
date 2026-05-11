#include "deploy/health_handler.h"
#include "test_framework.h"

TEST_CASE(deploy_features_verification) {
    // Feature 97: Health
    HealthHandler hh;
    ASSERT_EQ("{\"status\": \"UP\"}", hh.getHealthStatus());
    ASSERT_TRUE(hh.isLive());
}
