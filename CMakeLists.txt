cmake_minimum_required(VERSION 3.10)

project(RubiksCube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type was specified, building Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Add GLFW as subdirectory
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glfw-3.3.4/)

FILE(GLOB RUBIKSCUBE_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(
    RubiksCube 
    ${RUBIKSCUBE_SOURCES}
    ${CMAKE_SOURCE_DIR}/deps/glad/src/glad.c
)

# Enable compiler warnings
if (MSVC)
    target_compile_options(RubiksCube PRIVATE /W3)
else()
    target_compile_options(RubiksCube PRIVATE -Wall -Wextra)
endif()

target_link_libraries(RubiksCube glfw)

target_include_directories(
    RubiksCube PRIVATE
    ${CMAKE_SOURCE_DIR}/deps/glfw-3.3.4/include
    ${CMAKE_SOURCE_DIR}/deps/glm-0.9.9.8/include
    ${CMAKE_SOURCE_DIR}/deps/glad/include
)