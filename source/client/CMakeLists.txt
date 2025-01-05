cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
project(spumc LANGUAGES C)

include_directories(
    ../../external/sokol
    ../../external/doublenickel
    ../../external/gunslinger
    ../../external/handmade
    ../../external
)

set(CLIENT_SOURCES
    main.c
)
add_executable(spumc ${CLIENT_SOURCES})

target_compile_definitions(spumc PRIVATE SOKOL_GLES3=1)

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

set_target_properties(spumc PROPERTIES 
  COMPILE_FLAGS "-Os -std=c11"
  LINK_FLAGS "--shell-file ../../source/client/index.html -s WASM=1 -s MAX_WEBGL_VERSION=2 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 -Os"
)

set_target_properties(spumc PROPERTIES
    OUTPUT_NAME "index"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/spumc"
    SUFFIX ".html"
)