#include "api/api_versioning.h"
#include "api/auth_manager.h"
#include "api/rate_limiter.h"
#include "api/api_batching.h"
#include "api/distributed_tracer.h"
#include "test_framework.h"

TEST_CASE(api_features_verification) {
    // Feature 33: Versioning
    ApiVersioning av;
    av.registerRoute("v1", "/gen", "h1");
    ASSERT_EQ("h1", av.getHandler("v1", "/gen"));

    // Feature 34: Auth
    AuthManager am;
    am.addApiKey("key1", "admin");
    ASSERT_TRUE(am.authenticate("key1"));
    ASSERT_TRUE(am.isAuthorized("key1", "admin"));
    ASSERT_TRUE(!am.isAuthorized("key1", "super"));

    // Feature 35: Rate Limit
    RateLimiter rl(2, std::chrono::seconds(10));
    ASSERT_TRUE(rl.checkLimit("u1"));
    ASSERT_TRUE(rl.checkLimit("u1"));
    ASSERT_TRUE(!rl.checkLimit("u1"));

    // Feature 40: Tracing
    DistributedTracer dt;
    std::string id = dt.startSpan("op");
    ASSERT_TRUE(id.find("span-op") == 0);
}
