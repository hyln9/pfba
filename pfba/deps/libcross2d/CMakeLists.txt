cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT CMAKE_SYSTEM_NAME)
    set(CMAKE_SYSTEM_NAME "Generic")
endif (NOT CMAKE_SYSTEM_NAME)

# delete cmake cache folder before changing this options
option(BUILD_SFML "Build with SFML support" OFF)
option(BUILD_SDL2 "Build with SDL2 support" OFF)
option(BUILD_PSP2 "Build with PSP2 support" OFF)
option(BUILD_RPI "Build with RPI support" OFF)

if (BUILD_PSP2)
    set(VITASDK "/usr/local/vitasdk")
    set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
    set(CMAKE_C_COMPILER "${VITASDK}/bin/arm-vita-eabi-gcc")
    set(CMAKE_CXX_COMPILER "${VITASDK}/bin/arm-vita-eabi-g++")
    set(CMAKE_ASM_COMPILER "${VITASDK}/bin/arm-vita-eabi-g++")
    set(CMAKE_C_FLAGS "-Wl,-q" CACHE STRING "C flags")
    execute_process(COMMAND "date" "+%Y.%m.%d" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(TITLE_ID "CROSS2D00")
elseif (BUILD_RPI)
    set(RPISDK "/usr/local/rpi-tools")
    set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
    set(CMAKE_C_COMPILER "${RPISDK}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "${RPISDK}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++")
    set(CMAKE_ASM_COMPILER "${RPISDK}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++")
    set(CMAKE_C_FLAGS "-I${RPISDK}/sysroot/include" CACHE STRING "C flags")
endif (BUILD_PSP2)

project(cross2d)

# flags
set(FLAGS)

##############
# CROSS2D
##############
set(CROSS2D_DIR src src/skeleton)
file(GLOB CROSS2D_SRC src/skeleton/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (BUILD_PSP2)
    set(PLATFORM_DIRS
            src/psp2
            src/psp2/libvita2d/include
            src/psp2/vita-shader-collection/includes
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB PLATFORM_SRC
            src/psp2/*.c*
            src/psp2/libvita2d/source/*.c
            src/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            src/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            src/psp2/libvita2d/shader/compiled/*.o
            src/psp2/vita-shader-collection/lib/*.o
            )
    list(APPEND FLAGS -Wl,-q -O3 -D__PSP2__)
elseif (BUILD_SDL2)
    set(PLATFORM_DIRS src/sdl2)
    file(GLOB PLATFORM_SRC src/sdl2/*.c*)
    list(APPEND FLAGS -O3 -D__SDL2__)
elseif (BUILD_SFML)
    set(PLATFORM_DIRS src/sfml)
    file(GLOB PLATFORM_SRC
            src/sfml/*.c*
            src/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            )
    list(APPEND FLAGS -O3 -D__SFML__)
endif (BUILD_PSP2)


add_library(${PROJECT_NAME} ${CROSS2D_SRC} ${PLATFORM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CROSS2D_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PLATFORM_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})

#####################
# SDL2 (unix) target
#####################
add_executable(${PROJECT_NAME}.sdl2 src/main.cpp)
target_compile_options(${PROJECT_NAME}.sdl2 PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.sdl2
        cross2d SDL2 SDL2_image SDL2_ttf z)
add_dependencies(${PROJECT_NAME}.sdl2 ${PROJECT_NAME})

#####################
# SFML (unix) target
#####################
add_executable(${PROJECT_NAME}.sfml src/main.cpp)
target_compile_options(${PROJECT_NAME}.sfml PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.sfml
        cross2d SDL2 sfml-graphics sfml-window sfml-system GL png z)
add_dependencies(${PROJECT_NAME}.sfml ${PROJECT_NAME})

#####################
# PSP2 (vita) target
#####################
add_executable(${PROJECT_NAME}.psp2 src/main.cpp)
target_compile_options(${PROJECT_NAME}.psp2 PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.psp2
        psp2shell
        cross2d SDL2
        SceDisplay_stub SceGxm_stub SceSysmodule_stub
        SceCtrl_stub ScePgf_stub SceNetCtl_stub
        SceNet_stub ScePower_stub SceCommonDialog_stub
        SceAudio_stub SceAppMgr_stub
        stdc++ png jpeg z m c
        )
add_dependencies(${PROJECT_NAME}.psp2 ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}.vpk
        DEPENDS ${PROJECT_NAME}.psp2
        COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME}.psp2 ${PROJECT_NAME}.velf
        COMMAND ${VITASDK}/bin/vita-make-fself -c ${PROJECT_NAME}.velf eboot.bin
        )