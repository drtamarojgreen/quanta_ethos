cmake_minimum_required(VERSION 3.16)
project(PrismQuanta CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies using vcpkg
find_package(onnxruntime CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(sentencepiece CONFIG REQUIRED)
find_package(restinio CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
