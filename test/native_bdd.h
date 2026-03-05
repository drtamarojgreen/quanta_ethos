#pragma once

#include <sstream>
#include <string>

#include "test_framework.h"

#define SCENARIO(NAME) TEST_CASE(NAME)
#define GIVEN(DESC) const std::string given_step = std::string("GIVEN: ") + (DESC)
#define WHEN(DESC) const std::string when_step = std::string("WHEN: ") + (DESC)
#define THEN(DESC) const std::string then_step = std::string("THEN: ") + (DESC)

#define STEP_USED(STEP) ASSERT_TRUE(!(STEP).empty())

#define THEN_EQ(EXPECTED, ACTUAL) ASSERT_EQ((EXPECTED), (ACTUAL))
#define THEN_TRUE(EXPR) ASSERT_TRUE((EXPR))
