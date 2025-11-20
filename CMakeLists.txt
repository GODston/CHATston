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
    src/UI/src/UserInterface.cpp
    src/Models/Audio/src/Audio.cpp
    src/Models/Audio/src/Recorder.cpp
    src/Models/Audio/src/Player.cpp
    src/Views/Controls/AudioDeviceSelectors/src/AudioDeviceSelectors.cpp
)

# Build executable
add_executable(CHATston WIN32 ${SRCS})

# Add include directories so you can use clean paths
target_include_directories(CHATston PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(CHATston PRIVATE wx::core wx::net rtaudio)