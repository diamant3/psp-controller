cmake_minimum_required(VERSION 3.15)
project(psp-controller)

add_executable(
    ${PROJECT_NAME} 
    src/main.c
    src/callback.c
    src/network.c
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    pspdebug
    pspge
    pspdisplay
    pspnet
    pspnet_apctl
    pspctrl
)

create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
)
