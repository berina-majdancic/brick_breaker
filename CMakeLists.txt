cmake_minimum_required(VERSION 3.10)
project(brick_breaker)

set(CMAKE_CXX_STANDARD 11)
set(EXTERNAL_LIB_DIR "${PROJECT_SOURCE_DIR}/external/SDL2")
add_definitions(-DSDL_STATIC)
include_directories(${PROJECT_SOURCE_DIR}/external/SDL2/include ${PROJECT_SOURCE_DIR}/include )
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(brick_breaker ${SOURCES})
if (WIN32)
    target_link_libraries(brick_breaker PRIVATE ${EXTERNAL_LIB_DIR}/SDL2.lib)
elseif (UNIX)
    target_link_libraries(brick_breaker PRIVATE ${EXTERNAL_LIB_DIR}/libSDL2.a)
endif()
target_compile_definitions(brick_breaker PRIVATE SDL_MAIN_HANDLED)
    