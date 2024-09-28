#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define SAMPLEOS_TOTAL_INTERRUPTS 512

// 100 MB heap size
#define SAMPLEOS_HEAP_SIZE_BYTES 1024 * 1024 * 100
#define SAMPLEOS_HEAP_BLOCK_SIZE 4096
#define SAMPLEOS_HEAP_ADDRESS 0x01000000
#define SAMPLEOS_HEAP_TABLE_ADDRESS 0x00007E00

#define SAMPLEOS_SECTOR_SIZE 512

#define SAMPLEOS_MAX_FILESYSTEMS 12
#define SAMPLEOS_MAX_FILE_DESCRIPTORS 512

#define SAMPLEOS_MAX_PATH 108

#define SAMPLEOS_TOTAL_GDT_SEGMENTS 6

#define SAMPLEOS_PROGRAM_VIRTUAL_ADDRESS 0x400000
#define SAMPLEOS_USER_PROGRAM_STACK_SIZE 1024 * 16
#define SAMPLEOS_PROGRAM_VERTUAL_STACK_ADDRESS_START 0x3FF000
#define SAMPLEOS_PROGRAM_VERTUAL_STACK_ADDRESS_END \
  SAMPLEOS_PROGRAM_VERTUAL_STACK_ADDRESS_START -   \
      SAMPLEOS_USER_PROGRAM_STACK_SIZE

#define SAMPLEOS_MAX_PROGRAM_ALLOCATIONS 1024
#define SAMPLEOS_MAX_PROCESSES 12

#define USER_DATA_SEGMENT 0x23
#define USER_CODE_SEGMENT 0x1B

#endif  // CONFIG_H
