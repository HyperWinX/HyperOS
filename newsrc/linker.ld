OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
    text    PT_LOAD    FLAGS(5) ; /* Execute + Read */
    rodata  PT_LOAD    FLAGS(4) ;            /* Read only */
    data    PT_LOAD    FLAGS(6)  ; /* Write + Read */
    dynamic PT_DYNAMIC FLAGS(6) ; /* Dynamic PHDR for relocations */
}

SECTIONS
{
    /* We wanna be placed in the topmost 2GiB of the address space, for optimisations */
    /* and because that is what the Limine spec mandates. */
    /* Any address in this region will do, but often 0xffffffff80000000 is chosen as */
    /* that is the beginning of the region. */
    . = 0xffffffff80000000;
 
    .text : {
        *(.text .text.*)
    } :text
 
    /* Move to the next memory page for .rodata */
    . += CONSTANT(MAXPAGESIZE);
 
    .rodata : {
        *(.rodata .rodata.*)
    } :rodata
 
    /* Move to the next memory page for .data */
    . += CONSTANT(MAXPAGESIZE);
 
    .data : {
        *(.data .data.*)
    } :data
 
    /* Dynamic section for relocations, both in its own PHDR and inside data PHDR */
    .dynamic : {
        *(.dynamic)
    } :data :dynamic
 
    /* NOTE: .bss needs to be the last thing mapped to :data, otherwise lots of */
    /* unnecessary zeros will be written to the binary. */
    /* If you need, for example, .init_array and .fini_array, those should be placed */
    /* above this. */
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data
 
    /* Discard .note.* and .eh_frame since they may cause issues on some hosts. */
    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}