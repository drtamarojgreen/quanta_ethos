#include "data/json_logger.h"
#include "data/metrics_exporter.h"
#include "data/audit_logger.h"
#include "data/anonymizer.h"
#include "data/alert_manager.h"
#include "test_framework.h"

TEST_CASE(data_features_verification) {
    // Feature 41: JSON Log
    JsonLogger logger;
    ASSERT_EQ("{\"level\": \"info\", \"message\": \"test\"}", logger.formatJson("info", "test", {}));

    // Feature 43: Prometheus
    MetricsExporter me;
    me.incrementCounter("requests");
    ASSERT_EQ("requests_total 1\n", me.exposePrometheus());

    // Feature 45: Audit
    AuditLogger al("audit.log");
    al.logEvent("login", "jules", "success");
    ASSERT_EQ(1, al.getEntries().size());
    ASSERT_EQ("login", al.getEntries()[0].action);

    // Feature 46: Anonymizer
    Anonymizer an;
    ASSERT_EQ("User: [ANONYMOUS] said hello", an.anonymize("User: Jules said hello"));
}
