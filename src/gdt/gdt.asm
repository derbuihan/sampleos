
section .asm

global gdt_load

; void gdt_load(struct gdt* gdt, int size);
gdt_load:
    mov eax, [esp + 4] ; gdt
    mov [gdt_descriptor + 2], eax ; GDT Start Address
    mov ax, [esp + 8] ; size
    mov [gdt_descriptor], ax ; Size
    lgdt [gdt_descriptor]
    ret

section .data
gdt_descriptor:
    dw 0x0000 ; Size
    dw 0x0000 ; GDT Start Address
