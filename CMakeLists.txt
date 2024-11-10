cmake_minimum_required(VERSION 3.10)
project(brick_breaker)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(brick_breaker ${SOURCES})

target_link_libraries(brick_breaker ${SDL2_LIBRARIES})
