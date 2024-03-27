%macro do_check_edx 2
    lea esi, %1
    shr edi, 1
    mov eax, edx            ; Move edx value to eax
    and eax, edi            ; Apply AND
    cmp eax, edi            ; Check if bit 0 is set
    jne %2
    mov [esi], al
%endmacro
%macro do_check_ecx 2
    lea esi, %1
    shr edi, 1
    mov eax, ecx
    and eax, edi
    cmp eax, edi
    jne %2
    mov [esi], al
%endmacro

section .data
global cpu_bs, manufacturer
cpu_bs:
    times 49 db 0
manufacturer:
    times 13 db 0
fpu: db 0
vme: db 0
de: db 0
pse: db 0
tsc: db 0
msr: db 0
pae: db 0
mce: db 0
cx8: db 0
apic: db 0
sep: db 0
mtrr: db 0
pge: db 0
mca: db 0
cmov: db 0
pat: db 0
pse_36: db 0
psn: db 0
clfsh: db 0
nx: db 0
ds_ins: db 0
acpi: db 0
mmx: db 0
fxsr: db 0
sse: db 0
sse2: db 0
ss_ins: db 0
htt: db 0
tm: db 0
ia64: db 0
pbe: db 0
sse3: db 0
pclmulqdq: db 0
dtes64: db 0
monitor: db 0
ds_cpl: db 0
vmx: db 0
smx: db 0
est: db 0
tm2: db 0
ssse3: db 0
cnxt_id: db 0
sdbg: db 0
fma: db 0
cx16: db 0
xtpr: db 0
pdcm: db 0
pcid: db 0
dca: db 0
sse4_1: db 0
sse4_2: db 0
x2apic: db 0
movbe: db 0
popcnt_i: db 0
tsc_deadline: db 0
aes_ni: db 0
xsave: db 0
osxsave: db 0
avx: db 0
f16c: db 0
rdrnd: db 0
hypervisor: db 0

section .text
global RunCPUID
RunCPUID:
    pusha                   ; Save all general purpose register on stack

    mov eax, 80000000h      ; Load test value
    cpuid                   ; Run CPUID instruction
    mov edi, eax            ; Save maximum possible CPUID argument

    lea esi, manufacturer   ; Set string pointer
    xor eax, eax            ; First part - Get manufacturer string
    cpuid
    mov [esi], ebx          ; Start saving manufacturer string
    mov [esi+4], edx
    mov [esi+8], ecx
    ; cmp edi, 0x1              ; Check if we can get instructions list
    ; jle exit_failure         ; Exit, if not
    ; mov eax, 1              ; Set CPUID mode
    ; xor ecx, ecx            ; Zero out ecx
    ; xor edx, edx            ; Zero out edx
    ; cpuid
    ;call define_supported_instructions ; Reads all results 

    ;cmp edi, 80000004h      ; Check if we can get cpu brand string
    ;jl exit_failure         ; If eax is less, exit
    lea esi, cpu_bs
    mov eax, 80000002h      ; First part of brand string
    cpuid
    mov [esi], eax          ; Save first part
    mov [esi+4], ebx
    mov [esi+8], ecx
    mov [esi+12], edx
    mov eax, 80000003h
    cpuid
    mov [esi+16], eax       ; Save second part
    mov [esi+20], ebx
    mov [esi+24], ecx
    mov [esi+28], edx
    mov eax, 80000004h
    cpuid
    mov [esi+32], eax       ; Save third part
    mov [esi+36], ebx
    mov [esi+40], ecx
    mov [esi+44], edx
    popa
    xor eax, eax            ; Set 0 return value (success)
    ret                     ; Return

exit_failure:
    popa
    xor eax, eax
    mov eax, 1              ; Set 1 return value (failure)
    ret                     ; Return

define_supported_instructions:
    pusha                   ; Save all regs again
fpu_check:
    mov al, 1               ; Value for supported instructions
    mov edi, 0x8000000      ; Load value for checking, will be shr later
    lea esi, fpu            ; LEA fpu variable
    mov eax, edx            ; Move edx value to eax
    and eax, edi            ; Apply AND
    cmp eax, edi            ; Check if bit 0 is set
    jne vme_check           ; If not, jump to next
    mov [esi], al
vme_check: do_check_edx vme, de_check
de_check: do_check_edx de, pse_check
pse_check: do_check_edx pse, tsc_check
tsc_check: do_check_edx tsc, msr_check
msr_check: do_check_edx msr, pae_check
pae_check: do_check_edx pae, mce_check
mce_check: do_check_edx mce, cx8_check
cx8_check: do_check_edx cx8, apic_check
apic_check: do_check_edx apic, sep_check
sep_check: do_check_edx sep, mtrr_check
mtrr_check: do_check_edx mtrr, pge_check
pge_check: do_check_edx pge, mca_check
mca_check: do_check_edx mca, cmov_check
cmov_check: do_check_edx cmov, pat_check
pat_check: do_check_edx pat, pse_36_check
pse_36_check: do_check_edx pse_36, psn_check
psn_check: do_check_edx psn, clfsh_check
clfsh_check: do_check_edx clfsh, nx_check
nx_check: do_check_edx nx, ds_ins_check
ds_ins_check: do_check_edx ds_ins, acpi_check
acpi_check: do_check_edx acpi, mmx_check
mmx_check: do_check_edx mmx, fxsr_check
fxsr_check: do_check_edx fxsr, sse_check
sse_check: do_check_edx sse, sse2_check
sse2_check: do_check_edx sse2, ss_ins_check
ss_ins_check: do_check_edx ss_ins, htt_check
htt_check: do_check_edx htt, tm_check
tm_check: do_check_edx tm, ia64_check
ia64_check: do_check_edx ia64, pbe_check
pbe_check: do_check_ecx pbe, sse3_check
sse3_check: do_check_ecx sse3, pclmulqdq_check
pclmulqdq_check: do_check_ecx pclmulqdq, dtes64_check
dtes64_check: do_check_ecx dtes64, monitor_check
monitor_check: do_check_ecx monitor, ds_cpl_check
ds_cpl_check: do_check_ecx ds_cpl, vmx_check
vmx_check: do_check_ecx vmx, smx_check
smx_check: do_check_ecx smx, est_check
est_check: do_check_ecx est, tm2_check
tm2_check: do_check_ecx tm2, ssse3_check
ssse3_check: do_check_ecx ssse3, cnxt_id_check
cnxt_id_check: do_check_ecx cnxt_id, sdbg_check
sdbg_check: do_check_ecx sdbg, fma_check
fma_check: do_check_ecx fma, cx16_check
cx16_check: do_check_ecx cx16, xtpr_check
xtpr_check: do_check_ecx xtpr, pdcm_check
pdcm_check: do_check_ecx pdcm, pcid_check
pcid_check: do_check_ecx pcid, dca_check
dca_check: do_check_ecx dca, sse4_1_check
sse4_1_check: do_check_ecx sse4_1, sse4_2_check
sse4_2_check: do_check_ecx sse4_2, x2apic_check
x2apic_check: do_check_ecx x2apic, movbe_check
movbe_check: do_check_ecx movbe, popcnt_check
popcnt_check: do_check_ecx popcnt_i, tsc_deadline_check
tsc_deadline_check: do_check_ecx tsc_deadline, aes_ni_check
aes_ni_check: do_check_ecx aes_ni, xsave_check
xsave_check: do_check_ecx xsave, osxsave_check
osxsave_check: do_check_ecx osxsave, avx_check
avx_check: do_check_ecx avx, f16c_check
f16c_check: do_check_ecx f16c, rdrnd_check
rdrnd_check: do_check_ecx rdrnd, hypervisor_check
hypervisor_check:
    lea esi, hypervisor
    shr edi, 1
    mov eax, ecx
    and eax, edi
    cmp eax, edi
    jne exit
    mov [esi], al
exit:
    popa
    ret