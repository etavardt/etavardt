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

add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/scns ${PROJECT_SOURCE_DIR}/build/scns
        COMMENT "Copying scns" VERBATIM
        DEPENDS {SCNS})

add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/objs ${PROJECT_SOURCE_DIR}/build/objs
        COMMENT "Copying objs" VERBATIM
        DEPENDS {OBJS})

add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/maps ${PROJECT_SOURCE_DIR}/build/maps
        COMMENT "Copying maps" VERBATIM
        DEPENDS {MAPS})

add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bkgs ${PROJECT_SOURCE_DIR}/build/bkgs
        COMMENT "Copying bkgs" VERBATIM
        DEPENDS {BKGS})

add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/cols ${PROJECT_SOURCE_DIR}/build/cols
        COMMENT "Copying cols" VERBATIM
        DEPENDS {COLS})

add_custom_command(
        TARGET bob POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/srfs ${PROJECT_SOURCE_DIR}/build/srfs
        COMMENT "Copying srfs" VERBATIM
        DEPENDS {SRFS})
