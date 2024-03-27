#ifndef KERNEL_H
#define KERNEL_H

#include <common/stdint.h>
#include <kernel/idt/idt.h>

#define __WORDSIZE 32

// ----- Pre-processor constants -----
// IDT_SIZE: Specific to x86 architecture
#define IDT_SIZE 256
// KERNEL_CODE_SEGMENT_OFFSET: the first segment after the null segment in gdt.asm
#define KERNEL_CODE_SEGMENT_OFFSET 0x8
// 32-bit Interrupt gate: 0x8E
// ( P=1, DPL=00b, S=0, type=1110b => type_attr=1000_1110b=0x8E) (thanks osdev.org)
#define IDT_INTERRUPT_GATE_32BIT 0x8e
// IO Ports for PICs
#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1
// IO Ports for Keyboard
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define COMMAND_BUFFER_SIZE 100

extern char build_date[11];
extern char build_time[9];

// ----- External functions -----
extern void load_gdt();
extern void keyboard_handler();
extern char inb(unsigned short port);
extern void outb(unsigned short port, unsigned char data);

extern uint32_t inl(uint16_t port);
extern void outl(uint16_t port, uint32_t data);

extern void insl(uint16_t port, uint32_t buffer, uint32_t quads);

extern void LoadIDT(struct IDT_pointer* idt_address);
extern void InterruptsEnable();
extern void InterruptsDisable();


void disable_cursor();
void KeyboardInitialize();
void KeyboardKeyPress();
int kmain();

#endif