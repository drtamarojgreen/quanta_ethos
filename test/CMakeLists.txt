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
)

set(TEST_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/QuantaEthos
)

add_executable(prismquanta_unit_tests
    ${TEST_COMMON_SOURCES}
    unit/test_ethics_logic.cpp
    unit/test_model_backend.cpp
    unit/test_core_engine.cpp
)
target_include_directories(prismquanta_unit_tests PRIVATE ${TEST_INCLUDE_DIRS})
add_test(NAME prismquanta_unit_tests COMMAND prismquanta_unit_tests)

add_executable(prismquanta_native_bdd_tests
    ${TEST_COMMON_SOURCES}
    bdd/test_core_engine_bdd.cpp
)
target_include_directories(prismquanta_native_bdd_tests PRIVATE ${TEST_INCLUDE_DIRS})
add_test(NAME prismquanta_native_bdd_tests COMMAND prismquanta_native_bdd_tests)
