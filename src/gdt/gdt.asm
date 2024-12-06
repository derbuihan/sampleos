section .asm
global gdt_load

; void gdt_load(struct gdt* gdt, int size);
gdt_load:
    mov eax, [esp + 4]
    mov [gdt_descriptor + 2], eax
    mov ax, [esp + 8]
    mov [gdt_descriptor], ax
    lgdt [gdt_descriptor]
    ret

section .data
gdt_descriptor:
    dw 0 ; Size
    dd 0 ; GDT Start Address
