#include "ethos/ethos_config.h"
#include "logic/config_engine.h"
#include "test_framework.h"

TEST_CASE(regression_ethics_stability) {
    EthosConfig ec;
    auto framework = ec.getActiveFramework();
    ASSERT_EQ("default", framework->getName());
    ASSERT_EQ(1, framework->getRules().size());
    ASSERT_EQ("Do no harm.", framework->getRules()[0]);
}

TEST_CASE(regression_config_parser) {
    ConfigEngine ce;
    ce.load("nonexistent.txt");
    ASSERT_EQ("val", ce.get("key", "val"));
}
