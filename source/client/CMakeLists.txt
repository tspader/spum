set(CMAKE_TOOLCHAIN_FILE "${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake" CACHE FILEPATH "Emscripten toolchain file")

set(CLIENT_SOURCES
    main.c
)

add_executable(spumc ${CLIENT_SOURCES})

add_definitions(
  -DSOKOL_GLES3
)


set_target_properties(spumc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/client
)

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

set_target_properties(spumc PROPERTIES COMPILE_FLAGS "-Os")
set_target_properties(spumc PROPERTIES LINK_FLAGS    "-Os -s WASM=1 -s MAX_WEBGL_VERSION=2")
