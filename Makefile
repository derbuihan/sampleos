.PHONY: all clean

SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin

C_SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
ASM_SOURCES := $(shell find $(SRC_DIR) -type f -name '*.asm' ! -name 'boot.asm')

C_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.asm.o, $(ASM_SOURCES))

OBJS := $(ASM_OBJS) $(C_OBJS)

INCLUDES := -I./src
FLAGS := -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

CC := i686-elf-gcc
AS := nasm
LD := i686-elf-ld

all: $(BIN_DIR)/os.bin

$(BIN_DIR)/os.bin: $(BIN_DIR)/boot.bin $(BIN_DIR)/kernel.bin ./hello.txt
	@mkdir -p $(@D)
	rm -rf $(BIN_DIR)/os.bin
	dd if=$(BIN_DIR)/boot.bin >> $(BIN_DIR)/os.bin
	dd if=$(BIN_DIR)/kernel.bin >> $(BIN_DIR)/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> $(BIN_DIR)/os.bin # 1048576B = 1MB

	# Mount the image
	sudo mkdir -p /mnt/d
	sudo mount -t vfat $(BIN_DIR)/os.bin /mnt/d

	# Copy a file over
	sudo cp ./hello.txt /mnt/d/hello.txt
	sudo cp ./hello.txt /mnt/d/hello2.txt
	sudo cp ./hello.txt /mnt/d/hello3.txt
	sudo umount /mnt/d

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
