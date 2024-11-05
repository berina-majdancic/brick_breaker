# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(SDLProject LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the SDL2 library
find_package(SDL2 REQUIRED)

# Include the SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Add the source files
add_executable(sdl_window src/main.cpp)

# Link SDL2 library to the executable
target_link_libraries(sdl_window ${SDL2_LIBRARIES})
