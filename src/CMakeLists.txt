cmake_minimum_required(VERSION 3.14)

#-------------------------------------------------------------------------------
# Add source files
#-------------------------------------------------------------------------------
# NOTE: Please alphabetize these
target_sources(${PROJECT_NAME} PRIVATE 
    callbacks.cpp
    main.cpp
    math.cpp
    grid.cpp
    window.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}
)

#-------------------------------------------------------------------------------
# Add subdirectories
#-------------------------------------------------------------------------------
add_subdirectory(render)
add_subdirectory(cells)
