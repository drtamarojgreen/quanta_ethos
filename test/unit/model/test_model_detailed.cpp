#include "model/model_factory.h"
#include "model/model_registry.h"
#include "model/tensorrt_wrapper.h"
#include "model/quantization_util.h"
#include "model/batch_scheduler.h"
#include "model/health_monitor.h"
#include "model/model_cache.h"
#include "test_framework.h"

TEST_CASE(model_features_verification) {
    // Feature 12: Registry
    ModelRegistry mr;
    mr.loadModel("gpt-4");
    ASSERT_TRUE(mr.isLoaded("gpt-4"));
    ASSERT_EQ(1, mr.getLoadedCount());
    mr.unloadModel("gpt-4");
    ASSERT_TRUE(!mr.isLoaded("gpt-4"));

    // Feature 13: TensorRT
    TensorRTWrapper trt;
    ASSERT_EQ("TensorRT optimized output for: data", trt.runInference("data"));

    // Feature 16: Batching
    BatchScheduler bs;
    bs.addRequest({"p1", 10});
    bs.addRequest({"p2", 20});
    auto batch = bs.getNextBatch(1);
    ASSERT_EQ(1, batch.size());
    ASSERT_EQ("p2", batch[0].prompt); // Higher priority first

    // Feature 20: Cache
    ModelCache cache(10);
    cache.store("p1", "o1");
    std::string out;
    ASSERT_TRUE(cache.tryGet("p1", out));
    ASSERT_EQ("o1", out);
}
