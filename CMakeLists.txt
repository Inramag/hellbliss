cmake_minimum_required(VERSION 3.20)

project(game VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(engine)

add_executable(game
    src/main.cpp
)

target_include_directories(game PRIVATE
    include
)

target_link_libraries(game PRIVATE
    hellbliss
)

target_compile_options(hellbliss PRIVATE
    $<$<CONFIG:Release>:-O3>
)

target_link_options(hellbliss PRIVATE
    $<$<CONFIG:Release>:-s>
    $<$<CONFIG:Release>:-flto>
)

target_compile_options(game PRIVATE
    $<$<CONFIG:Release>:-O3>
)

target_link_options(game PRIVATE
    $<$<CONFIG:Release>:-s>
    $<$<CONFIG:Release>:-flto>
)
