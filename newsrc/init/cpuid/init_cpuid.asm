bits 64

section .data
vendor: extern vendor
cpu_brandstring: extern cpu_brandstring

section .text

global _read_vendor
global _read_cpu_brandstring

_read_vendor:
    xor eax, eax
    ; Read vendor string
    cpuid
    mov [vendor], ebx
    mov [vendor + 4], edx
    mov [vendor + 8], ecx
    ret

_read_cpu_brandstring:
    ; Read CPU brand string
    mov eax, 0x80000002
    cpuid
    mov [cpu_brandstring], eax
    mov [cpu_brandstring + 4], ebx
    mov [cpu_brandstring + 8], ecx
    mov [cpu_brandstring + 12], edx

    mov eax, 0x80000003
    cpuid
    mov [cpu_brandstring + 16], eax
    mov [cpu_brandstring + 20], ebx
    mov [cpu_brandstring + 24], ecx
    mov [cpu_brandstring + 28], edx

    mov eax, 0x80000004
    cpuid
    mov [cpu_brandstring + 32], eax
    mov [cpu_brandstring + 36], ebx
    mov [cpu_brandstring + 40], ecx
    mov [cpu_brandstring + 44], edx
    ret
