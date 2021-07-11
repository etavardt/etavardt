include_directories(.)

add_executable(img2tga IMG2TGA.CPP)

add_test(
    NAME test_img2tga
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
    COMMAND img2tga venus3
)
