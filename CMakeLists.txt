cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(CHATston LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxBUILD_SHARED OFF)

# Disable wxWidgets components you don't need
set(wxBUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)
set(wxBUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(wxBUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

# Only build the libraries you actually use
set(wxUSE_STL ON CACHE BOOL "" FORCE)
set(wxUSE_LIBJPEG OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBTIFF OFF CACHE BOOL "" FORCE)
# Keep PNG support for UI images (icons, buttons, etc)
# set(wxUSE_LIBPNG OFF CACHE BOOL "" FORCE)
set(wxUSE_ZLIB builtin CACHE STRING "" FORCE)
set(wxUSE_EXPAT builtin CACHE STRING "" FORCE)
set(wxUSE_REGEX builtin CACHE STRING "" FORCE)

message(STATUS "Fetching wxWidgets...")

FetchContent_Declare(
   wxWidgets
   GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
   GIT_SHALLOW ON
)
FetchContent_MakeAvailable(wxWidgets)

set(SRCS src/main.cpp)

add_executable(CHATston WIN32 ${SRCS})
target_link_libraries(CHATston PRIVATE wx::core wx::net)