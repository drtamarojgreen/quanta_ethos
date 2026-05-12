# Dummy targets for optional dependencies so the demo can build without vendored packages.
add_library(sentencepiece::sentencepiece INTERFACE IMPORTED)
add_library(onnxruntime::onnxruntime INTERFACE IMPORTED)
add_library(restinio::restinio INTERFACE IMPORTED)
add_library(nlohmann_json::nlohmann_json INTERFACE IMPORTED)

add_subdirectory(QuantaEthos)

file(GLOB_RECURSE PRISMQUANTA_LIB_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
list(REMOVE_ITEM PRISMQUANTA_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QuantaEthos/main.cpp
)

set(PRISMQUANTA_INCLUDE_DIRS
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
    ${CMAKE_CURRENT_SOURCE_DIR}/QuantaEthos
)

add_library(prismquanta_lib ${PRISMQUANTA_LIB_SOURCES})
target_include_directories(prismquanta_lib PUBLIC ${PRISMQUANTA_INCLUDE_DIRS})

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
