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
option(BUILD_DEBUG   "Build with debug settings"        ON)
option(USE_ASAN      "Build with address sanitization"  ON)

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

if(BUILD_DEBUG AND USE_ASAN)
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=undefined,address)
    target_link_options(${PROJECT_NAME} PRIVATE /fsanitize=undefined,address)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined,address)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined,address)
  endif()
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
include_directories(${OPENGL_INCLUDE_DIR})
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
# Declare it as a static library
add_library(glad STATIC
  ${GLAD_SOURCE_DIR}/src/gl.c
)
target_include_directories(glad PRIVATE ${GLAD_SOURCE_DIR}/include)
# Include it for use in project
target_include_directories(${PROJECT_NAME} PRIVATE
  ${GLAD_SOURCE_DIR}/include
)
# Link it to project
target_link_libraries(${PROJECT_NAME} glad)


#-------------------------------------------------------------------------------
# GLM
#-------------------------------------------------------------------------------
# Download GLFW from git repo
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
# Declare it as a library
FetchContent_MakeAvailable(glm)

# Link it to project
target_link_libraries(${PROJECT_NAME} glm)

#-------------------------------------------------------------------------------
# Add source
#-------------------------------------------------------------------------------
add_subdirectory(src)
