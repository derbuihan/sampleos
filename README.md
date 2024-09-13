# SampleOS

SampleOS is a simple operating system that I am developing for educational purposes.
It is written in C and Assembly and is based on the x86 architecture.

# Requirements

- CLion
- Docker
- `qemu`
- `nasm`
- `i686-elf-binutils`
- `i686-elf-gcc`

# Build the OS

```bash
docker run --privileged --rm -it -v "$(pwd):/tmp/sampleos" -w /tmp/sampleos clion/ubuntu/cpp-env:1.0 /bin/bash -c "make clean; make all"
```

# Run the OS

```bash
$ qemu-system-x86_64 -hda bin/os.bin
```

# Debug the OS

```bash
$ i386-elf-gdb
(gdb) add-symbol-file ./build/kernelfull.o 0x100000
(gdb) substitute-path /tmp/sampleos /path/to/sampleos # If you are using a different path
(gdb) target remote | qemu-system-i386 -hda ./bin/os.bin -S -gdb stdio
(gdb) break kernel.c:86 # Breakpoint at the kernel main function
(gdb) c
(gdb) layout asm
(gdb) stepi
```

# References

- [PeachOS](https://github.com/nibblebits/PeachOS)
- [OSDev Wiki](https://wiki.osdev.org/Main_Page)
