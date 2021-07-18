include_directories(
    src/
)

file(GLOB SOURCE_FILES LIST_DIRECTORIES false
    src/*.cpp
)

file(GLOB HEADER_FILES LIST_DIRECTORIES false
    src/*.hpp
)

#set(CMAKE_VERBOSE_MAKEFILE TRUE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0 -pg -no-pie")
add_executable(bob ${SOURCE_FILES} ${HEADER_FILES})

foreach(dir IN ITEMS "bkgs" "cols" "maps" "objs" "scns" "srfs")
    add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${dir} ${PROJECT_SOURCE_DIR}/build/${dir}
        COMMENT "Copying ${dir}" VERBATIM
    )
endforeach()

add_custom_command(
    TARGET bob POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/venus3_tst.img ${PROJECT_SOURCE_DIR}/build/
    COMMENT "Copying venus3_tst.img" VERBATIM
)
add_custom_command(
    TARGET bob POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeTest.bat ${PROJECT_SOURCE_DIR}/build/test_bob.bat
    COMMENT "Copying CMakeTest.bat" VERBATIM
)
