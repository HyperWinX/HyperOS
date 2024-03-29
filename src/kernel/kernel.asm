bits 32

section .multiboot

dd 0xE85250D6	    ; Magic number
dd 0x0			    ; Flags
dd 0x0F             ; Header length
dd - (0xE85250D6 + 0x0 + 0x0F)	; Checksum

section .data
; Include the GDT from previous tutorials
; Set this as our GDT with LGDT
; instead of relying on what the bootloader sets up for us
gdt_start:
gdt_null:	; Entry 1: Null entry must be included first (error check)
	dd 0x0	; double word = 4 bytes = 32 bits
	dd 0x0
gdt_kernel_code:	; Entry 2: Kernel code segment
	dw 0xFFFF		; Limit bits 0-15
	dw 0x0000		; Base low bits 16-31
	db 0x00			; Base middle bits 32-39
	db 0x9A			; Access bits 40-47
	db 0xCF			; Flags bits 48-55
	db 0x00			; Base high bits 56-63

gdt_kernel_data:	; Entry 3: Kernel data segment
	dw 0xFFFF		; Limit bits 0-15
	dw 0x0000		; Base low bits 16-31
	db 0x00			; Base middle bits 32-39
	db 0x92			; Access bits 40-47
	db 0xCF			; Flags bits 48-55
	db 0x00			; Base high bits 56-63

gdt_end:			; Needed to calculate GDT size for inclusion in GDT descriptor

; GDT Descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1	; Size of GDT, always less one
	dd gdt_start

section .text

; Define constants
CODE_SEG equ gdt_kernel_code - gdt_start
DATA_SEG equ gdt_kernel_data - gdt_start

; Make global anything that is used in C files
global start
global load_gdt
global LoadIDT
global keyboard_handler
global inb
global outb
global inl
global outl
global insl
global InterruptsEnable
global InterruptsDisable

extern kmain			; Defined in kernel.c
extern KeyboardKeyPress

load_gdt:
	lgdt [gdt_descriptor] ; from gdt.asm
	ret

LoadIDT:
	mov edx, [esp + 4]
	lidt [edx]
	ret

InterruptsEnable:
	sti
	ret

InterruptsDisable:
    cli
    ret

keyboard_handler:
	pushad
	cld
	call KeyboardKeyPress
	popad
	iretd

inb:
	mov edx, [esp + 4] ; PORT_TO_READ, 16 bits
	; dx is lower 16 bits of edx. al is lower 8 bits of eax
	; Format: in <DESTINATION_REGISTER>, <PORT_TO_READ>
	in al, dx					 ; Read from port DX. Store value in AL
	; Return will send back the value in eax
	; (al in this case since return type is char, 8 bits)
	ret

outb:
	mov edx, [esp + 4]	; port to write; DST_IO_PORT. 16 bits
	mov eax, [esp + 8] 	; value to write. 8 bits
	; Format: out <DST_IO_PORT>, <VALUE_TO_WRITE>
	out dx, al
	ret

inl:
	mov edx, [esp + 4]
	in eax, dx
	ret

outl:
	mov edx, [esp + 4]
	mov eax, [esp + 8]
	out dx, eax
	ret

insl:
    popa
    mov edx, [esp + 4]
    mov ebx, [esp + 8]
    mov ecx, [esp + 12]
    xor edi, edi
next_iteration:
    in eax, dx
    mov [ebx + edi], eax
    dec ecx
    cmp ecx, 0
    jne next_iteration
    ret

start:
	; THANK YOU MICHAEL PETCH
	; https://stackoverflow.com/questions/62885174/multiboot-keyboard-driver-triple-faults-with-grub-works-with-qemu-why
	lgdt [gdt_descriptor]
	mov ax, DATA_SEG          ; Setup the segment registers with our flat data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, stack_space        ; set stack pointer
	cli				; Disable interrupts
	mov esp, stack_space
	jmp kmain
	hlt

section .bss
resb 8192			; 8KB for stack
stack_space:
