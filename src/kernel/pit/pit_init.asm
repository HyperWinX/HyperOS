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
    push eax
    push ebx
    mov eax, [IRQ0_fractions]
    mov ebx, [IRQ0_ms];
    add [system_timer_fractions], eax
    adc [system_timer_ms], ebx
    mov al, 0x20
    out 0x20, al
    pop ebx
    pop eax
    iretd
global pit_init
pit_init:
    nop
    pushad
    mov eax, 0x10000
    cmp ebx, 18
    jbe .gotReloadValue

    mov eax, 1
    cmp ebx, 1193181
    jae .gotReloadValue

    mov eax, 3579545
    mov edx, 0
    div ebx
    cmp edx, 3579545 / 2
    jb .l1
    inc eax
.l1:
    mov ebx, 3
    mov edx, 0
    div ebx
    cmp edx, 3 / 2
    jb .l2
    inc eax
.l2:

.gotReloadValue:
    push eax
    mov [PIT_reload_value], ax
    mov ebx, eax
    mov eax, 3579545
    mov edx, 0
    div ebx
    cmp edx, 3579545 / 2
    jb .l3
    inc eax
.l3:
    mov ebx, 3
    mov edx, 0
    div ebx
    cmp edx, 3 / 2
    jb .l4
    inc eax
.l4:
    mov [IRQ0_frequency], eax
    pop ebx
    mov eax, 0xDBB3A062
    mul ebx
    shrd eax, edx, 10
    shr edx, 10
    mov [IRQ0_ms], edx
    mov [IRQ0_fractions], eax

    cli

    mov al, 00110100b
    out 0x43, al

    mov ax, [PIT_reload_value]
    out 0x40, al
    mov al, ah
    out 0x40, al

    nop
    popad
    ret

global TimerIRQ
TimerIRQ:
    push eax
    mov eax, [CountDown]
    test eax, eax
    jz TimerDone
    dec eax
    mov [CountDown], eax
TimerDone:
    pop eax
    iretd
