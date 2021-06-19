cmake_minimum_required(VERSION 3.10)

# set the project name
project(Bob C CXX)

# specify the C++ standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
    src/
)

file(GLOB SOURCE_FILES LIST_DIRECTORIES false
    src/*.C
    src/*.cpp
)

file(GLOB HEADER_FILES LIST_DIRECTORIES false
    src/*.H
    src/*.hpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2")

# add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy bob.exe ..)