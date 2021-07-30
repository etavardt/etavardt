#include(CMakePrintHelpers)
#Example usage: cmake_print_variables(SOURCE_FILES)

include_directories(
    src/
)

file(GLOB SOURCE_FILES LIST_DIRECTORIES false
    src/*.cpp
)

file(GLOB HEADER_FILES LIST_DIRECTORIES false
    src/*.hpp
)

add_executable(bob ${SOURCE_FILES} ${HEADER_FILES})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(bob PUBLIC "SHELL:-O0 -pg -no-pie -fno-builtin")
    target_link_options(bob PUBLIC "SHELL:-O0 -pg -no-pie -fno-builtin" "LINKER:-static,-demangle,--disable-dynamicbase,--disable-reloc-section") #this works as well
    #target_link_options(bob PUBLIC "SHELL:-O0 -pg -no-pie -fno-builtin" "LINKER:-t,-static,-demangle,--disable-dynamicbase,--disable-reloc-section") #this works as well and -t is for trace or listing items used
    #target_link_options(bob PUBLIC -O0 -pg -no-pie -fno-builtin "LINKER:-t,-static,-demangle,--disable-dynamicbase,--disable-reloc-section") #this works as well
endif()

foreach(dir IN ITEMS "bkgs" "cols" "maps" "objs" "scns" "srfs")
    add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${dir} ${PROJECT_SOURCE_DIR}/build/${dir}
        COMMENT "Copying ${dir}" VERBATIM
    )
endforeach()

# TomC -- Unit Testing
enable_testing()

file(GLOB TEST_SOURCE_FILES LIST_DIRECTORIES false test/*.cpp)
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*main.cpp$")
add_executable(bob_test ${TEST_SOURCE_FILES} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(bob_test gtest_main)

include(GoogleTest)
gtest_discover_tests(bob_test)

# #set(CMAKE_CXX_CPPCHECK "cppcheck")
# find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
# if (CMAKE_CXX_CPPCHECK)
#     message("-- CppCheck found : ${CMAKE_CXX_CPPCHECK}")
#     message("-- current source dir : ${CMAKE_CURRENT_SOURCE_DIR}")
#     message("-- source dir : ${CMAKE_SOURCE_DIR}")
#     list(
#         APPEND CMAKE_CXX_CPPCHECK
#             "--enable=warning,style,performance,portability,information,missingInclude"
#             "--inconclusive"
#             "--force"
#             "--inline-suppr"
#             "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
#     )
# endif()

# find_program(CMAKE_CXX_CPPCHECK cppcheck NAMES cppcheck HINTS $ENV{PROGRAMFILES}/cppcheck)

# cmake_print_variables(CPP_CHECK_CMD)
# cmake_print_variables(CMAKE_CXX_CPPCHECK)
# cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

# # If CppCheck executable found
# if(CMAKE_CXX_CPPCHECK)
#   # Check CppCheck version
#   set(CPP_CHECK_CMD ${CMAKE_CXX_CPPCHECK} --version)
#   execute_process(COMMAND ${CPP_CHECK_CMD}
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     RESULT_VARIABLE CPP_CHECK_RESULT
#     OUTPUT_VARIABLE CPP_CHECK_VERSION
#     ERROR_VARIABLE CPP_CHECK_ERROR)

#     cmake_print_variables(CPP_CHECK_RESULT)

#   # Check if version could be extracted
#   if(CPP_CHECK_RESULT EQUAL 0)
#     # Get number of CPU cores
#     include(ProcessorCount)
#     ProcessorCount(CPU_CORES)

#     cmake_print_variables(CPU_CORES)
#     cmake_print_variables(CMAKE_SOURCE_DIR)

#     # Append desired arguments to CppCheck
#     list(
#       APPEND CMAKE_CXX_CPPCHECK
#         # Using the below template will allow jumping to any found error from inside Visual Studio output window by double click
#         "--template \"{file}({line}): {severity} ({id}): {message}\""
#         # Use all the available CPU cores
#         "-j ${CPU_CORES}"
#         # Only show found errors
#         "--quiet"
#         # Desired warning level in CppCheck
#         "--enable=all" # all or warning,style,performance,portability,information,unusedFunction,missingInclude
#         # Optional: Specified C++ version
#         "--std=c++17"
#         # Optional: Specified platform
#         "--platform=win64"
#         # Optional: suppression file stored in same directory as the top level CMake script
#         #"--suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt"
#         # Optional: Use inline suppressions
#         "--inline-suppr"
#         # Run CppCheck from the working directory, as specified in the add_custom_target command below
#         "${CMAKE_CURRENT_SOURCE_DIR}/src"
#       )
#       cmake_print_variables(CMAKE_CXX_CPPCHECK)

#     add_custom_target(CPPCHECK
#       COMMAND ${CMAKE_CXX_CPPCHECK}
#       DEPENDS bob
#       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#       COMMENT "Static code analysis using ${CPP_CHECK_VERSION}"
#     )
#   endif()
# endif()
