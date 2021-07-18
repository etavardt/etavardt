cmake_minimum_required(VERSION 3.17)

project(BobRT LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0")

include(CTest)
enable_testing()

add_subdirectory(rt)
add_subdirectory(tools)

add_test(
    NAME test_bob_bat
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
    COMMAND test_bob.bat
)
