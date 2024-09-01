.PHONY: all clean

SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin

C_SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
ASM_SOURCES := $(shell find $(SRC_DIR) -type f -name '*.asm' ! -name 'boot.asm')

C_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.asm.o, $(ASM_SOURCES))

OBJS := $(C_OBJS) $(ASM_OBJS)

INCLUDES := -I./src
FLAGS := -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

CC := i686-elf-gcc
AS := nasm
LD := i686-elf-ld

all: $(BIN_DIR)/os.bin

$(BIN_DIR)/os.bin: $(BIN_DIR)/boot.bin $(BIN_DIR)/kernel.bin
	@mkdir -p $(@D)
	rm -rf $(BIN_DIR)/os.bin
	dd if=$(BIN_DIR)/boot.bin >> $(BIN_DIR)/os.bin
	dd if=$(BIN_DIR)/kernel.bin >> $(BIN_DIR)/os.bin
	dd if=/dev/zero bs=512 count=100 >> $(BIN_DIR)/os.bin

$(BIN_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	@mkdir -p $(@D)
	$(AS) -f bin $< -o $@

$(BIN_DIR)/kernel.bin: $(OBJS)
	@mkdir -p $(@D)
	$(LD) -g -relocatable $(OBJS) -o $(BUILD_DIR)/kernelfull.o
	$(CC) $(FLAGS) -T $(SRC_DIR)/linker.ld -o $@ -ffreestanding -O0 -nostdlib $(BUILD_DIR)/kernelfull.o

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDES) -I$(dir $<) $(FLAGS) -std=gnu99 -c $< -o $@

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(@D)
	$(AS) -f elf -g $< -o $@

clean:
	rm -f ./bin/boot.bin
	rm -f ./bin/kernel.bin
	rm -f ./bin/os.bin
	rm -f ./build/kernelfull.o
	rm -f $(OBJS)
