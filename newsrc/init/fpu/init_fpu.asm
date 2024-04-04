bits 64

section .data
fpu_test: dw 0x55AA

section .text

global fpu_exists
fpu_exists:
    ; Get CR0 copy
    mov rdx, cr0
    ; Zero flags
    and rdx, (-1) - (0b1000 + 0b0100)
    ; Set CR0
    mov cr0, rdx
    ; Try to init FPU
    fninit
    ; Save test word
    fnstsw [fpu_test]
    ; Compare written status with expected FPU state
    cmp word [fpu_test], 0
    jne no_fpu
    xor eax, eax
    mov eax, 1
    ret
no_fpu:
    xor eax, eax
    ret