ORG 0
BITS 16

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0x7c0:step2

; Interrupt handler
handle_zero:
    mov ah, 0eh
    mov al, 'A'
    int 10h
    iret

step2:
    cli; Disable interrupts
    ; Set data segment to 0x7c0
    mov ax, 0x7c0
    mov ds, ax
    ; Set stack segment to 0x0000
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00 ; Set stack pointer to 0x7c0
    sti; Enable interrupts

    ; interrupt vector table
    mov word[ss:0x00], handle_zero
    mov word[ss:0x02], 0x7c0
    int 0 ; Trigger interrupt

    mov si, message
    call print
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 10h
    ret

message: db "Hello, World!", 0

times 510-($-$$) db 0
dw 0xAA55
