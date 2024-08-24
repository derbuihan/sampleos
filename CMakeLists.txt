cmake_minimum_required(VERSION 3.14)
project(sampleos C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_COMPILER /opt/cross/bin/i686-elf-gcc)
set(CMAKE_ASM_COMPILER nasm)

add_custom_command(
        OUTPUT boot.bin
        COMMAND ${CMAKE_ASM_COMPILER} -o boot.bin ${CMAKE_SOURCE_DIR}/src/boot.asm
        DEPENDS ${CMAKE_SOURCE_DIR}/src/boot.asm
)

add_custom_target(sampleos DEPENDS boot.bin)
