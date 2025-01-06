cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
project(spums LANGUAGES C)

add_executable(spums)

set(SP_SOURCE_DIR ${SP_PROJECT_ROOT}/source)
set(SP_INCLUDE_DIR ${SP_PROJECT_ROOT}/external/include)
set(SP_LIB_DIR ${SP_PROJECT_ROOT}/external/lib)
set(SP_FILE_DIR $<TARGET_FILE_DIR:spums>)
set(SP_SOURCES main.c)

if (WIN32)
    set(SP_LWS_DLL ${SP_LIB_DIR}/libwebsockets.dll)
    set(SP_LIBS libwebsockets)
elseif (UNIX) 
    set(SP_LIBS libwebsockets.a ssl crypto cap)
endif()


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

target_link_directories(spums PRIVATE
    ${SP_LIB_DIR}
)

target_link_libraries(spums PRIVATE 
    ${SP_LIBS}
)

if (WIN32)
    add_custom_command(TARGET spums 
        POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SP_LWS_DLL}
        $<TARGET_FILE_DIR:spums>
    )
endif()