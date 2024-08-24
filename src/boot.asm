ORG 0
BITS 16

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0x7c0:step2

step2:
    cli; Disable interrupts
    ; Set data segment to 0x7c0
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    ; Set stack segment to 0x0000
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00 ; Set stack pointer to 0x7c0
    sti; Enable interrupts

    ; Load the second sector
    mov ah, 2 ; Read sectors
    mov al, 1 ; Read one sector
    mov ch, 0 ; Cylinder 0
    mov cl, 2 ; Read second sector
    mov dh, 0 ; Head number
    mov bx, buffer ; Buffer address
    int 0x13 ; BIOS interrupt

    jc error

    mov si, message
    call print

    mov si, buffer
    call print
    jmp $

error:
    mov si, error_message
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

message db "Hello, Kernel!", 0

error_message db "Error loading the second sector", 0

times 510-($-$$) db 0
dw 0xAA55

buffer: