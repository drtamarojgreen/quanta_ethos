#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class TestFailure : public std::runtime_error {
public:
    explicit TestFailure(const std::string& message)
        : std::runtime_error(message) {}
};

struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

inline int register_test(const std::string& name, std::function<void()> fn) {
    registry().push_back({name, std::move(fn)});
    return 0;
}

#define TEST_CASE(NAME) \
    void NAME(); \
    static int NAME##_registered = register_test(#NAME, NAME); \
    void NAME()

#define ASSERT_TRUE(EXPR) \
    do { \
        if (!(EXPR)) { \
            std::ostringstream oss; \
            oss << "Expected true: " << #EXPR << " at " << __FILE__ << ":" << __LINE__; \
            throw TestFailure(oss.str()); \
        } \
    } while (false)

#define ASSERT_EQ(EXPECTED, ACTUAL) \
    do { \
        const auto expected_value = (EXPECTED); \
        const auto actual_value = (ACTUAL); \
        if (!(expected_value == actual_value)) { \
            std::ostringstream oss; \
            oss << "Assertion failed at " << __FILE__ << ":" << __LINE__ \
                << " expected [" << expected_value << "] but got [" << actual_value << "]"; \
            throw TestFailure(oss.str()); \
        } \
    } while (false)

#define ASSERT_NEAR(EXPECTED, ACTUAL, EPSILON) \
    do { \
        const auto expected_value = static_cast<double>(EXPECTED); \
        const auto actual_value = static_cast<double>(ACTUAL); \
        const auto epsilon_value = static_cast<double>(EPSILON); \
        if (std::fabs(expected_value - actual_value) > epsilon_value) { \
            std::ostringstream oss; \
            oss << "Assertion failed at " << __FILE__ << ":" << __LINE__ \
                << " expected [" << expected_value << "] got [" << actual_value << "] with epsilon [" << epsilon_value << "]"; \
            throw TestFailure(oss.str()); \
        } \
    } while (false)

inline int run_all_tests() {
    int failures = 0;

    for (const auto& test : registry()) {
        try {
            test.fn();
            std::cout << "[PASS] " << test.name << '\n';
        } catch (const std::exception& ex) {
            ++failures;
            std::cerr << "[FAIL] " << test.name << " -- " << ex.what() << '\n';
        }
    }

    std::cout << "Executed " << registry().size() << " tests, failures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
