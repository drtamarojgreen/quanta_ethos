#include "sec/input_sanitizer.h"
#include "sec/secrets_vault.h"
#include "sec/dos_guard.h"
#include "test_framework.h"

TEST_CASE(sec_features_verification) {
    // Feature 71: Sanitizer
    InputSanitizer is;
    ASSERT_EQ("ls -l", is.sanitize("ls -l; rm -rf /"));

    // Feature 75: Secrets
    SecretsVault sv;
    sv.storeSecret("db", "pass");
    ASSERT_EQ("pass", sv.getSecret("db"));

    // Feature 77: DoS
    DosMitigator dm(100);
    ASSERT_TRUE(dm.isAllowed("127.0.0.1"));
}
