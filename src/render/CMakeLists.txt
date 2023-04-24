cmake_minimum_required(VERSION 3.14)

#-------------------------------------------------------------------------------
# Add source files
#-------------------------------------------------------------------------------
# NOTE: Please alphabetize these
target_sources(${PROJECT_NAME} PRIVATE
    quad.cpp
    shader.cpp
)
