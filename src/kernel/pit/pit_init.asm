%macro pusha 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push rbp
%endmacro
%macro popa 0
    pop rbp
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

section .bss
system_timer_fractions: resd 1
system_timer_ms:        resd 1
IRQ0_fractions:         resd 1
IRQ0_ms:                resd 1
IRQ0_frequency:         resd 1
PIT_reload_value:       resw 1
section .data
CountDown: dw 0
section .text
IRQ0_handler:
    push rax
    push rbx
    mov rax, [IRQ0_fractions]
    mov rbx, [IRQ0_ms];
    add [system_timer_fractions], rax
    adc [system_timer_ms], rbx
    mov al, 0x20
    out 0x20, al
    pop rbx
    pop rax
    iretd
global pit_init
pit_init:
    nop
    pusha
    mov rax, 0x10000
    cmp rbx, 18
    jbe .gotReloadValue

    mov rax, 1
    cmp rbx, 1193181
    jae .gotReloadValue

    mov rax, 3579545
    mov rdx, 0
    div rbx
    cmp rdx, 3579545 / 2
    jb .l1
    inc rax
.l1:
    mov rbx, 3
    mov rdx, 0
    div rbx
    cmp rdx, 3 / 2
    jb .l2
    inc rax
.l2:

.gotReloadValue:
    push rax
    mov [PIT_reload_value], ax
    mov rbx, rax
    mov rax, 3579545
    mov rbx, 0
    div rbx
    cmp rdx, 3579545 / 2
    jb .l3
    inc rax
.l3:
    mov rbx, 3
    mov rdx, 0
    div rbx
    cmp rdx, 3 / 2
    jb .l4
    inc rax
.l4:
    mov [IRQ0_frequency], rax
    pop rbx
    mov rax, 0xDBB3A062
    mul rbx
    shrd rax, rdx, 10
    shr rdx, 10
    mov [IRQ0_ms], rdx
    mov [IRQ0_fractions], rax

    cli

    mov al, 00110100b
    out 0x43, al

    mov ax, [PIT_reload_value]
    out 0x40, al
    mov al, ah
    out 0x40, al

    nop
    popa
    ret

global TimerIRQ
TimerIRQ:
    push rax
    mov rax, [CountDown]
    test rax, rax
    jz TimerDone
    dec rax
    mov [CountDown], rax
TimerDone:
    pop rax
    iretd
