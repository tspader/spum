cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
project(spum LANGUAGES C)

# Set the executable name
set(EXECUTABLE_NAME spum)

# Specify source files
set(SOURCES
    source/main.c
)

# Specify include directories
include_directories(
    external/sokol
    external/doublenickel
    external/gunslinger
    external/handmade
)

# Add compile definitions
add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    -D_SILENCE_CXX17_ALL_DEPRECATION_WARNINGS
    -DSOKOL_GLCORE
)

# Add compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zi /Od /JMC /MDd /std:c11 /EHa /W3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4530 /wd4201 /wd4577 /wd4310 /wd4624 /wd4099 /wd4068 /wd4267 /wd4244 /wd4018")

# Add linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:FULL /MACHINE:X64 /NOLOGO /SUBSYSTEM:CONSOLE /INCREMENTAL:NO /NOIMPLIB /NOEXP")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /PDB:./intermediate/")

# Specify libraries to link
set(LIBRARIES
    user32.lib
    opengl32.lib
    gdi32.lib
    Shell32.lib
    Kernel32.lib
    Advapi32.lib
    Ole32.lib
    OleAut32.lib
)

# Specify intermediate directory for object and pdb files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./intermediate/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./intermediate/)

# Add the executable and link libraries
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBRARIES})
