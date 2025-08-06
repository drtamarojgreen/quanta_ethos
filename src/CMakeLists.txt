add_subdirectory(QuantaEthos)

add_library(prismquanta_lib
    core_engine.cpp
    model_backend.cpp
)

target_include_directories(prismquanta_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

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
