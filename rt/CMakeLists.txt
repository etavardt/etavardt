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

file(GLOB SCNS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/scns/*)
file(GLOB OBJS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/objs/*)
file(GLOB MAPS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/maps/*)
file(GLOB BKGS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/bkgs/*)
file(GLOB COLS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/cols/*)
file(GLOB SRFS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/srfs/*)

add_executable(bob ${SOURCE_FILES} ${HEADER_FILES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

foreach(dir IN ITEMS "bkgs" "cols" "maps" "objs" "scns" "srfs")
    add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${dir} ${PROJECT_SOURCE_DIR}/build/${dir}
        COMMENT "Copying ${dir}" VERBATIM)       
endforeach()
