add_subdirectory(QuantaEthos)

add_library(prismquanta_lib
    core_engine.cpp
    model_backend.cpp
    logic/plugin_loader.cpp
    logic/session_manager.cpp
    logic/async_engine.cpp
    logic/config_engine.cpp
    logic/command_history.cpp
    logic/template_processor.cpp
    logic/resource_guard.cpp
    logic/locale_registry.cpp
    logic/variant_manager.cpp
    logic/graph_connector.cpp
    model/model_factory.cpp
    model/model_registry.cpp
    model/tensorrt_wrapper.cpp
    model/quantization_util.cpp
    model/spec_decoder.cpp
    model/batch_scheduler.cpp
    model/finetune_interface.cpp
    model/hw_accel.cpp
    model/health_monitor.cpp
    model/model_cache.cpp
    ethos/xai_provider.cpp
    ethos/ethos_config.cpp
    ethos/bias_scanner.cpp
    ethos/source_attributor.cpp
    ethos/uncertainty_logic.cpp
    ethos/feedback_manager.cpp
    ethos/toxicity_filter.cpp
    ethos/tone_analyzer.cpp
    ethos/pii_redactor.cpp
    ethos/self_correction.cpp
    api/grpc_service.cpp
    api/streaming_handler.cpp
    api/api_versioning.cpp
    api/auth_manager.cpp
    api/rate_limiter.cpp
    api/webhook_handler.cpp
    api/api_batching.cpp
    api/openapi_gen.cpp
    api/sdk_generator.cpp
    api/distributed_tracer.cpp
    data/json_logger.cpp
    data/log_aggregator.cpp
    data/metrics_exporter.cpp
    data/retention_manager.cpp
    data/audit_logger.cpp
)

target_include_directories(prismquanta_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Dummy targets for missing dependencies to allow tests to link
add_library(sentencepiece::sentencepiece INTERFACE IMPORTED)
add_library(onnxruntime::onnxruntime INTERFACE IMPORTED)
add_library(restinio::restinio INTERFACE IMPORTED)
add_library(nlohmann_json::nlohmann_json INTERFACE IMPORTED)

target_link_libraries(prismquanta_lib
    PRIVATE
    sentencepiece::sentencepiece
    onnxruntime::onnxruntime
)

add_executable(prismquanta main.cpp)

target_link_libraries(prismquanta
    PRIVATE
    prismquanta_lib
    restinio::restinio
    nlohmann_json::nlohmann_json
)
