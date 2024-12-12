#ifndef SAMPLEOS_IDT_H
#define SAMPLEOS_IDT_H

#include <stdint.h>

struct interrupt_frame;
typedef void* (*ISR80H_COMMAND)(struct interrupt_frame* frame);

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

struct interrupt_frame {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t reserved;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t ip;
  uint32_t cs;
  uint32_t flags;
  uint32_t esp;
  uint32_t ss;
} __attribute__((packed));

void idt_init();
extern void enable_interrupts();
extern void disable_interrupts();
void isr80h_register_command(int command_id, ISR80H_COMMAND command);

#endif  // SAMPLEOS_IDT_H
