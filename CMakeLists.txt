cmake_minimum_required(VERSION 3.10)
project(BrickBreaker)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS} include)
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()

# Find PkgConfig
find_package(PkgConfig REQUIRED)

# Find SDL2_ttf using PkgConfig
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
if (SDL2_TTF_FOUND)
    include_directories(${SDL2_TTF_INCLUDE_DIRS})
    link_directories(${SDL2_TTF_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

# Copy the font directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/font DESTINATION ${CMAKE_BINARY_DIR})

# Gather all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(BrickBreaker ${SOURCES})

# Link SDL2 and SDL2_ttf libraries
target_link_libraries(BrickBreaker ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# Optionally, set the runtime path to find SDL2 libraries on UNIX systems
if(UNIX)
    set_target_properties(BrickBreaker PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "$ORIGIN"
    )
endif()
