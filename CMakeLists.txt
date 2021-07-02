cmake_minimum_required(VERSION 3.17)

# set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(BobRT LANGUAGES C CXX)

add_subdirectory(rt)
add_subdirectory(tools)
