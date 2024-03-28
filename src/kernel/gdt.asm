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
	db 0xFC			; Limit bits 48-51 and flags bits 52-55
	db 0x00			; Base high bits 56-63

gdt_kernel_data:	; Entry 3: Kernel data segment
	dw 0xFFFF		; Limit bits 0-15
	dw 0x0000		; Base low bits 16-31
	db 0x00			; Base middle bits 32-39
	db 0x92			; Access bits 40-47
	db 0xFC			; Flags bits 48-55
	db 0x00			; Base high bits 56-63

gdt_end:			; Needed to calculate GDT size for inclusion in GDT descriptor

; GDT Descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1	; Size of GDT, always less one
	dd gdt_start