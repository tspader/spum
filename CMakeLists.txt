cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
project(spum LANGUAGES C)

include_directories(
    external/sokol
    external/doublenickel
    external/gunslinger
    external/handmade
    external
)

add_subdirectory(source/client)
add_subdirectory(source/server)
