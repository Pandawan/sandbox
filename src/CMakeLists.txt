cmake_minimum_required(VERSION 3.14)

#-------------------------------------------------------------------------------
# Add source files
#-------------------------------------------------------------------------------
target_sources(${PROJECT_NAME} PRIVATE 
    main.cpp
    callbacks.cpp
    window.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}
)

add_subdirectory(pixels)