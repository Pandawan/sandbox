cmake_minimum_required(VERSION 3.14)
include(FetchContent)
project(
    sandbox
    VERSION 1.0.0
    LANGUAGES CXX C
)

#-------------------------------------------------------------------------------
# Build options
#-------------------------------------------------------------------------------
option(BUILD_DEBUG     "Build with debug settings"    ON)

if (BUILD_DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(BUILD_DEBUG ON CACHE BOOL "Build with debug settings" FORCE)
endif()

#-------------------------------------------------------------------------------
# Make an executable target
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
# Compiler warnings & errors
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic -Werror)
endif()

#-------------------------------------------------------------------------------
# Output config
#-------------------------------------------------------------------------------
# Control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

#-------------------------------------------------------------------------------
# Base dependencies
#-------------------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)


#-------------------------------------------------------------------------------
# GLFW
#-------------------------------------------------------------------------------
# Only build GLFW as a static library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

# Download GLFW from git repo
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
# Declare it as a library
FetchContent_MakeAvailable(glfw)

# Link it to project
target_link_libraries(${PROJECT_NAME} glfw)

#-------------------------------------------------------------------------------
# GLAD
#-------------------------------------------------------------------------------
set(GLAD_SOURCE_DIR "deps/glad")
# Include it for use in project
include_directories(${PROJECT_NAME} PUBLIC
    ${GLAD_SOURCE_DIR}/include
)
# Declare it as a static library
add_library(glad STATIC
    ${GLAD_SOURCE_DIR}/src/gl.c
    ${GLAD_SOURCE_DIR}/include
)
# Link it to project
target_link_libraries(${PROJECT_NAME} glad)


#-------------------------------------------------------------------------------
# GLFWPP
#-------------------------------------------------------------------------------
set(GLFWPP_SOURCE_DIR "deps/glfwpp")
# Include it for use in project
add_subdirectory(${GLFWPP_SOURCE_DIR})
# Link it to project
target_link_libraries(${PROJECT_NAME} GLFWPP)


#-------------------------------------------------------------------------------
# Add source
#-------------------------------------------------------------------------------
add_subdirectory(src)
