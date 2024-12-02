[BITS 32]
section .asm

global task_return
global restore_general_purpose_registers
global user_registers

; void task_return(struct registers* regs);
task_return:
    mov ebp, esp
    ; PUSH THE DATA SEGMENT (SS WILL BE FINE)
    ; PUSH THE STACK ADDRESS
    ; PUSH THE FLAGS
    ; PUSH THE CODE SEGMENT
    ; PUSH IP

    ; Let's access the structure passed to us
    mov ebx, [ebp + 4]
    ; Push the data/stack selector
    push dword [ebx + 44]
    ; Push the stack pointer
    push dword [ebx + 40]

    ; Push the flags
    pushf
    pop eax
    or eax, 0x200
    push eax

    ; Push the code segment
    push dword [ebx+32]

    ; Push the IP to execute
    push dword [ebx+28]

    ; Setup some segment registers
    mov ax, [ebx + 44]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword [ebx + 4]
    call restore_general_purpose_registers
    add esp, 4

    ; Let's leave kernel land and execute in user land
    iretd

; void restore_general_purpose_registers(struct registers* regs);
restore_general_purpose_registers:
    push ebp
    mov ebp, esp

    mov ebx, [ebp + 8] ; struct registers* regs
    mov edi, [ebx + 0] ; edi
    mov esi, [ebx + 4] ; esi
    mov ebp, [ebx + 8] ; ebp
    mov ebx, [ebx + 12] ; ebx
    mov edx, [ebx + 16] ; edx
    mov ecx, [ebx + 20] ; ecx
    mov eax, [ebx + 24] ; eax

    pop esp
    ret

; void user_registers()
user_registers:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret
