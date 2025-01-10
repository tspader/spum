cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

project(spumc LANGUAGES C)

add_executable(spumc)

set(SP_SOURCE_DIR ${SP_PROJECT_ROOT}/source)
set(SP_INCLUDE_DIR ${SP_PROJECT_ROOT}/external/include)
set(SP_FILE_DIR $<TARGET_FILE_DIR:spumc>)
set(SP_SOURCES main.c)

target_sources(spumc PRIVATE
    ${SP_SOURCES}
)

target_include_directories(spumc PRIVATE
    ${SP_SOURCE_DIR}
    ${SP_INCLUDE_DIR}
    ${SP_INCLUDE_DIR}/sokol
    ${SP_INCLUDE_DIR}/sokol/util
    ${SP_INCLUDE_DIR}/doublenickel
    ${SP_INCLUDE_DIR}/gunslinger
    ${SP_INCLUDE_DIR}/handmade
    ${SP_INCLUDE_DIR}/Nuklear
)


target_compile_definitions(spumc PRIVATE SOKOL_GLES3=1)

target_compile_options(spumc PRIVATE
    "-std=c11"
    "-O0"
)

target_link_options(spumc PRIVATE
    "-sWASM=1"
    "-sUSE_WEBGL2=1"
    "-sMAX_WEBGL_VERSION=2"
    "-sALLOW_MEMORY_GROWTH=1"
    "--shell-file=../../source/client/index.html"
    "-lwebsocket.js"
)

set_target_properties(spumc PROPERTIES
    OUTPUT_NAME "index"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/spumc"
    SUFFIX ".html"
)
