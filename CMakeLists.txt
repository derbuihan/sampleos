cmake_minimum_required(VERSION 3.14)
project(sampleos C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_COMPILER /opt/cross/bin/i686-elf-gcc)
set(CMAKE_ASM_COMPILER /opt/cross/bin/i686-elf-as)

add_custom_command(
        OUTPUT boot.o
        COMMAND ${CMAKE_ASM_COMPILER} -o boot.o ${CMAKE_SOURCE_DIR}/src/boot.s -g
        DEPENDS ${CMAKE_SOURCE_DIR}/src/boot.s
)

add_custom_command(
        OUTPUT kernel.o
        COMMAND ${CMAKE_C_COMPILER} -o kernel.o -c ${CMAKE_SOURCE_DIR}/src/kernel.c -g -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        DEPENDS ${CMAKE_SOURCE_DIR}/src/kernel.c
)

add_custom_command(
        OUTPUT sampleos.bin
        COMMAND ${CMAKE_C_COMPILER} -o sampleos.bin -T ${CMAKE_SOURCE_DIR}/src/linker.ld boot.o kernel.o -g -m32 -ffreestanding -O2 -nostdlib -lgcc
        DEPENDS boot.o kernel.o
)

add_custom_target(sampleos  DEPENDS sampleos.bin)
