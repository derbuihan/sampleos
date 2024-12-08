all: ./bin/blank.bin

./build/blank.o: ./blank.asm
	@mkdir -p $(@D)
	nasm -f elf ./blank.asm -o ./build/blank.o

./bin/blank.bin: ./build/blank.o
	@mkdir -p $(@D)
	i686-elf-gcc -g -T ./linker.ld -o ./bin/blank.bin -ffreestanding -O0 -nostdlib -fpic ./build/blank.o

clean:
	rm -rf ./build/blank.o ./bin/blank.bin