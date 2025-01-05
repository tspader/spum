set(SERVER_SOURCES
    main.c
)
add_executable(spums ${SERVER_SOURCES})
set_target_properties(spums PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/server
)
