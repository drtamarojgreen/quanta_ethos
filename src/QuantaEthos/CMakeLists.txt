add_library(quanta_ethos_lib ethics_logic.cpp)
target_include_directories(quanta_ethos_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(quanta_ethos main.cpp)
target_link_libraries(quanta_ethos PRIVATE quanta_ethos_lib nlohmann_json::nlohmann_json)
