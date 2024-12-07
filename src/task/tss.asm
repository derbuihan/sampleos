section .asm

global tss_load

; void tss_load(int tss_segment);
tss_load:
    push ebp
    mov ebp, esp
    mov ax, [ebp + 8]
    ltr ax
    pop ebp
    ret

