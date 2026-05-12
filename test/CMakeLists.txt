cmake_minimum_required(VERSION 3.16)
project(prismquanta_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

set(TEST_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/api
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/data
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/deploy
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/dev
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/ethos
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/logic
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/model
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/perf
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/sec
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/ux
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/QuantaEthos
)

add_executable(prismquanta_unit_tests
    test_main.cpp
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
target_link_libraries(prismquanta_unit_tests PRIVATE prismquanta_lib)
add_test(NAME prismquanta_unit_tests COMMAND prismquanta_unit_tests)

add_executable(prismquanta_bdd_tests
    test_main.cpp
    bdd/test_full_suite.cpp
)
target_include_directories(prismquanta_bdd_tests PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(prismquanta_bdd_tests PRIVATE prismquanta_lib)
add_test(NAME prismquanta_bdd_tests COMMAND prismquanta_bdd_tests)

add_executable(prismquanta_sdd_tests
    test_main.cpp
    sdd/cards/structural_enforcement_detailed.cpp
)
target_include_directories(prismquanta_sdd_tests PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(prismquanta_sdd_tests PRIVATE prismquanta_lib)
add_test(NAME prismquanta_sdd_tests COMMAND prismquanta_sdd_tests)
