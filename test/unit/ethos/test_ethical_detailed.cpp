#include "ethos/xai_provider.h"
#include "ethos/bias_scanner.h"
#include "ethos/source_attributor.h"
#include "ethos/uncertainty_logic.h"
#include "ethos/feedback_manager.h"
#include "ethos/toxicity_filter.h"
#include "ethos/pii_redactor.h"
#include "test_framework.h"

TEST_CASE(ethical_features_verification) {
    // Feature 21: XAI
    XAIProvider xai;
    xai.addReason({"Accuracy", true, "High confidence"});
    ASSERT_TRUE(xai.generateExplanation().find("PASS") != std::string::npos);

    // Feature 23: Bias
    BiasScanner bs;
    auto res = bs.scan("exclusive content");
    ASSERT_TRUE(res.detected);
    ASSERT_EQ("Inclusivity", res.category);

    // Feature 25: Uncertainty
    UncertaintyLogic ul(0.7f);
    ASSERT_EQ("I am sorry, but I do not have enough certain information to answer that accurately.", ul.processResponse("ans", 0.5f));
    ASSERT_EQ("ans", ul.processResponse("ans", 0.9f));

    // Feature 27: Toxicity
    ToxicityFilter tf;
    ASSERT_TRUE(tf.isToxic("This is toxic."));
    ASSERT_EQ("This is [REDACTED].", tf.scrub("This is toxic."));

    // Feature 29: PII
    PiiRedactor pr;
    ASSERT_EQ("Contact [EMAIL_REDACTED]", pr.redact("Contact test@example.com"));
}
