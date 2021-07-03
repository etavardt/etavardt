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

add_executable(bob ${SOURCE_FILES} ${HEADER_FILES})

foreach(dir IN ITEMS "bkgs" "cols" "maps" "objs" "scns" "srfs")
    add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${dir} ${PROJECT_SOURCE_DIR}/build/${dir}
        COMMENT "Copying ${dir}" VERBATIM)       
endforeach()
