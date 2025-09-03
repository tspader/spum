cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)

project(spum-cli LANGUAGES C)

add_executable(spum-cli)

set(SP_SOURCE_DIR  ${SP_PROJECT_ROOT}/source)
set(SP_CLI_DIR     ${SP_PROJECT_ROOT}/source/cli)
set(SP_INCLUDE_DIR ${SP_PROJECT_ROOT}/external/include)
set(SP_SOURCES main.c)
set(SP_LIBS m luajit-5.1)

target_sources(spum-cli PRIVATE
    ${SP_SOURCES}
)

message(${SP_INCLUDE_DIR})

target_include_directories(spum-cli PRIVATE
    ${SP_SOURCE_DIR}
    ${SP_INCLUDE_DIR}
    ${SP_INCLUDE_DIR}/sokol
    ${SP_INCLUDE_DIR}/doublenickel
    ${SP_INCLUDE_DIR}/gunslinger
    ${SP_INCLUDE_DIR}/handmade
)

target_link_libraries(spum-cli PRIVATE 
    ${SP_LIBS}
)

target_compile_options(spum-cli PRIVATE
    "-O0"
)
    
target_link_options(spum-cli PRIVATE)

add_custom_command(
	TARGET spum-cli
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${SP_CLI_DIR}/cli.lua ${CMAKE_CURRENT_BINARY_DIR}/cli.lua)