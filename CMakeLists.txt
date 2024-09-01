cmake_minimum_required(VERSION 3.14)
project(sampleos C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_COMPILER i686-elf-gcc)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_LINKER i686-elf-ld)

set(INCLUDES "-I${CMAKE_SOURCE_DIR}/src")
set(FLAGS -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/boot)

add_custom_command(
        OUTPUT os.bin
        COMMAND dd if=boot/boot.bin > os.bin
        COMMAND dd if=kernel.bin >> os.bin
        COMMAND dd if=/dev/zero bs=512 count=100 >> os.bin
        DEPENDS boot/boot.bin kernel.bin
)

add_custom_command(
        OUTPUT boot/boot.bin
        COMMAND ${CMAKE_ASM_COMPILER} -f bin -o boot/boot.bin ${CMAKE_SOURCE_DIR}/src/boot/boot.asm
        DEPENDS ${CMAKE_SOURCE_DIR}/src/boot/boot.asm
)

add_custom_command(
        OUTPUT kernel.asm.o
        COMMAND ${CMAKE_ASM_COMPILER} -f elf -g -o kernel.asm.o ${CMAKE_SOURCE_DIR}/src/kernel.asm
        DEPENDS ${CMAKE_SOURCE_DIR}/src/kernel.asm
)

add_custom_command(
        OUTPUT kernel.bin
        COMMAND ${CMAKE_LINKER} -g -relocatable -o kernelfull.o kernel.asm.o kernel.o
        COMMAND ${CMAKE_C_COMPILER} ${FLAGS} -T ${CMAKE_SOURCE_DIR}/src/linker.ld -o kernel.bin kernelfull.o -O0 -ffreestanding -nostdlib
        DEPENDS ${CMAKE_SOURCE_DIR}/src/linker.ld kernel.asm.o kernel.o
)

add_custom_command(
        OUTPUT kernel.o
        COMMAND ${CMAKE_C_COMPILER} ${FLAGS} -c -o kernel.o ${CMAKE_SOURCE_DIR}/src/kernel.c ${INCLUDES} -std=gnu99
        DEPENDS ${CMAKE_SOURCE_DIR}/src/kernel.c
)

add_custom_target(sampleos DEPENDS os.bin)
