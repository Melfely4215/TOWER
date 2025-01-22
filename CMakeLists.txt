cmake_minimum_required(VERSION 3.27)
project(TOWER)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(COPY fonts/ DESTINATION ${CMAKE_BINARY_DIR}/bin)

add_executable(main src/main.cpp "src/enemy.h" "src/enemy.cpp" "src/turret.h" "src/turret.cpp" "src/wave.h" "src/wave.cpp")
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
