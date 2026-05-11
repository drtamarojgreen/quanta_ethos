cmake_minimum_required(VERSION 3.16)
project(prismquanta_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

set(TEST_COMMON_SOURCES
    test_main.cpp
    ../src/core_engine.cpp
    ../src/model_backend.cpp
    ../src/QuantaEthos/ethics_logic.cpp
    ../src/logic/plugin_loader.cpp
    ../src/logic/session_manager.cpp
    ../src/logic/async_engine.cpp
    ../src/logic/config_engine.cpp
    ../src/logic/command_history.cpp
    ../src/logic/template_processor.cpp
    ../src/logic/resource_guard.cpp
    ../src/logic/locale_registry.cpp
    ../src/logic/variant_manager.cpp
    ../src/logic/graph_connector.cpp
    ../src/model/model_factory.cpp
    ../src/model/model_registry.cpp
    ../src/model/tensorrt_wrapper.cpp
    ../src/model/quantization_util.cpp
    ../src/model/spec_decoder.cpp
    ../src/model/batch_scheduler.cpp
    ../src/model/finetune_interface.cpp
    ../src/model/hw_accel.cpp
    ../src/model/health_monitor.cpp
    ../src/model/model_cache.cpp
    ../src/ethos/xai_provider.cpp
    ../src/ethos/ethos_config.cpp
    ../src/ethos/bias_scanner.cpp
    ../src/ethos/source_attributor.cpp
    ../src/ethos/uncertainty_logic.cpp
    ../src/ethos/feedback_manager.cpp
    ../src/ethos/toxicity_filter.cpp
    ../src/ethos/tone_analyzer.cpp
    ../src/ethos/pii_redactor.cpp
    ../src/ethos/self_correction.cpp
    ../src/api/grpc_service.cpp
    ../src/api/streaming_handler.cpp
    ../src/api/api_versioning.cpp
    ../src/api/auth_manager.cpp
    ../src/api/rate_limiter.cpp
    ../src/api/webhook_handler.cpp
    ../src/api/api_batching.cpp
    ../src/api/openapi_gen.cpp
    ../src/api/sdk_generator.cpp
    ../src/api/distributed_tracer.cpp
    ../src/data/json_logger.cpp
    ../src/data/log_aggregator.cpp
    ../src/data/metrics_exporter.cpp
    ../src/data/retention_manager.cpp
    ../src/data/audit_logger.cpp
)

set(TEST_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/QuantaEthos
)

add_executable(prismquanta_unit_tests
    ${TEST_COMMON_SOURCES}
    unit/logic/test_logic_detailed.cpp
    unit/model/test_model_detailed.cpp
    unit/ethos/test_ethical_detailed.cpp
    unit/api/test_api_detailed.cpp
    unit/data/test_data_detailed.cpp
    unit/dev/test_dev_detailed.cpp
    unit/ux/test_ux_detailed.cpp
    unit/sec/test_sec_detailed.cpp
    unit/perf/test_perf_detailed.cpp
    unit/deploy/test_deploy_detailed.cpp
    unit/test_regression_all.cpp
)
target_include_directories(prismquanta_unit_tests PRIVATE ${TEST_INCLUDE_DIRS})
add_test(NAME prismquanta_unit_tests COMMAND prismquanta_unit_tests)

add_executable(prismquanta_bdd_tests
    ${TEST_COMMON_SOURCES}
    bdd/test_full_suite.cpp
)
target_include_directories(prismquanta_bdd_tests PRIVATE ${TEST_INCLUDE_DIRS})
add_test(NAME prismquanta_bdd_tests COMMAND prismquanta_bdd_tests)

add_executable(prismquanta_sdd_tests
    ${TEST_COMMON_SOURCES}
    sdd/cards/structural_enforcement_detailed.cpp
)
target_include_directories(prismquanta_sdd_tests PRIVATE ${TEST_INCLUDE_DIRS})
add_test(NAME prismquanta_sdd_tests COMMAND prismquanta_sdd_tests)
