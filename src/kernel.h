#ifndef SAMPLEOS_KERNEL_H
#define SAMPLEOS_KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define SAMPLEOS_MAX_PATH 108

void kernel_main();
void print(const char *str);

#endif  // SAMPLEOS_KERNEL_H
