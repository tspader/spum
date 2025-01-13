cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)

project(spums LANGUAGES C)

add_executable(spums)

set(SP_SOURCE_DIR ${SP_PROJECT_ROOT}/source)
set(SP_INCLUDE_DIR ${SP_PROJECT_ROOT}/external/include)
set(SP_FILE_DIR $<TARGET_FILE_DIR:spums>)
set(SP_SOURCES main.c)
set(SP_LIBS websockets ssl crypto cap z pthread)

target_sources(spums PRIVATE
    ${SP_SOURCES}
)

message(${SP_INCLUDE_DIR})

target_include_directories(spums PRIVATE
    ${SP_SOURCE_DIR}
    ${SP_INCLUDE_DIR}
    ${SP_INCLUDE_DIR}/sokol
    ${SP_INCLUDE_DIR}/doublenickel
    ${SP_INCLUDE_DIR}/gunslinger
    ${SP_INCLUDE_DIR}/handmade
    ${SP_INCLUDE_DIR}/libwebsockets
)

target_link_libraries(spums PRIVATE 
    ${SP_LIBS}
)

target_compile_options(spums PRIVATE
    "-O0"
    "-fsanitize=address"
)
    
target_link_options(spums PRIVATE
    "-fsanitize=address"
)