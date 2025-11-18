cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Set runtime library BEFORE project() command
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

project(CHATston LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include configuration files
include(cmake/wxWidgetsConfig.cmake)
include(cmake/RtAudioConfig.cmake)

# Fetch dependencies
include(cmake/FetchDependencies.cmake)

# Source files
set(SRCS 
    src/main.cpp
    src/Models/Audio/Recorder.cpp
)

# Build executable
add_executable(CHATston WIN32 ${SRCS})
target_link_libraries(CHATston PRIVATE wx::core wx::net rtaudio)