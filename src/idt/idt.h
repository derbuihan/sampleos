#ifndef SAMPLEOS_IDT_H
#define SAMPLEOS_IDT_H

#include <stdint.h>

struct idt_desc {
  uint16_t offset_1;  // offset bits 0..15
  uint16_t selector;  // Selector that is in our GDT
  uint8_t zero;       // Does nothing, unused set to 0
  uint8_t type_attr;  // Descriptor type and attributes
  uint16_t offset_2;  // offset bits 16..31
} __attribute__((packed));

struct idtr_desc {
  uint16_t limit;  // Size of the IDT
  uint32_t base;   // Base address of the start of the IDT
} __attribute__((packed));

void idt_init();

#endif  // SAMPLEOS_IDT_H
