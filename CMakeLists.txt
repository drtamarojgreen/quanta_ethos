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

enable_testing()
add_subdirectory(test)
