cmake_minimum_required(VERSION 3.16)
project(prismquanta LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

# Add sorrel CLI
add_executable(sorrel
    src/dev/sorrel_main.cpp
    src/dev/sdd_engine.cpp
)
target_include_directories(sorrel PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Project setup target
add_custom_target(setup
    COMMAND ${CMAKE_COMMAND} -E make_directory queue/pending queue/in_progress queue/completed queue/failed queue/human_approval
    COMMAND ${CMAKE_COMMAND} -E make_directory actions/pending actions/in_progress actions/failed
    COMMAND ${CMAKE_COMMAND} -E make_directory logs
    COMMAND ${CMAKE_COMMAND} -E make_directory bin
    COMMAND ${CMAKE_COMMAND} -E touch prismquanta.env
    COMMENT "Setting up project directory structure and environment..."
)

enable_testing()
add_subdirectory(test)

# Integration test target
add_custom_target(test_integration
    COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure
    DEPENDS prismquanta quanta_ethos sorrel
    COMMENT "Running all tests via CTest..."
)
