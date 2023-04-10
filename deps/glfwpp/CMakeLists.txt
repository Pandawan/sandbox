cmake_minimum_required(VERSION 3.1)
project(GLFWPP CXX)

#Create library target
add_library(GLFWPP INTERFACE)
target_include_directories(GLFWPP INTERFACE include)
target_compile_features(GLFWPP INTERFACE cxx_std_17)
