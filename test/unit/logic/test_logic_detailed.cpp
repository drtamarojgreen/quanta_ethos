#include "logic/plugin_loader.h"
#include "logic/session_manager.h"
#include "logic/async_engine.h"
#include "logic/config_engine.h"
#include "logic/command_history.h"
#include "logic/template_processor.h"
#include "logic/resource_guard.h"
#include "logic/locale_registry.h"
#include "logic/variant_manager.h"
#include "logic/graph_connector.h"
#include "test_framework.h"

TEST_CASE(logic_features_verification) {
    // Feature 2: Session
    SessionManager sm;
    auto session = sm.getOrCreateSession("test_id");
    ASSERT_EQ("test_id", session->getId());

    // Feature 4: Config
    ConfigEngine ce;
    ce.load("nonexistent.txt");
    ASSERT_EQ("default", ce.get("key", "default"));

    // Feature 6: Templates
    TemplateProcessor tp;
    std::map<std::string, std::string> vars = {{"name", "Jules"}};
    ASSERT_EQ("Hello Jules", tp.process("Hello {{name}}", vars));

    // Feature 7: Resources
    ResourceGuard rg(2);
    ASSERT_TRUE(rg.tryAcquire());
    ASSERT_TRUE(rg.tryAcquire());
    ASSERT_TRUE(!rg.tryAcquire());
    rg.release();
    ASSERT_TRUE(rg.tryAcquire());
}
