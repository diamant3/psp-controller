cmake_minimum_required(VERSION 3.0)
project(psp-controller)

add_compile_options(-Wall -Wextra -Werror -Wconversion)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)
add_executable(
    ${PROJECT_NAME} 
    src/main.c
    src/callback.c
    src/connection.c
    src/psp_modules.c
)

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
