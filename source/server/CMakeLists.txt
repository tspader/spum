cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
project(spums LANGUAGES C)

include_directories(
    ../../external/sokol
    ../../external/doublenickel
    ../../external/gunslinger
    ../../external/handmade
    ../../external
)

set(SERVER_SOURCES
    main.c
)
add_executable(spums ${SERVER_SOURCES})