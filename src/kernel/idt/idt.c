#include <kernel/idt/idt.h>
#include <kernel/pit/pit.h>
#include <common/stdint.h>
#include <kernel/kernel.h>
#include <common/stdio.h>

char* exception_messages[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",
        "Double fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment not present",
        "Stack fault",
        "General protection fault",
        "Page fault",
        "Unknown Interrupt",
        "Coprocessor Fault",
        "Alignment Fault",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};
struct IDT_entry IDT[IDT_SIZE];

void InitIDT() {
	InitIDTDesc(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);
	// Send ICWs - Initialization Command Words
	// PIC1: IO Port 0x20 (command), 0xA0 (data)
	// PIC2: IO Port 0x21 (command), 0xA1 (data)
	// ICW1: Initialization command
	// Send a fixed value of 0x11 to each PIC to tell it to expect ICW2-4
	// Restart PIC1
    outb(PIC1_COMMAND_PORT, 0x11);
    outb(PIC2_COMMAND_PORT, 0x11);
	// ICW2: Vector Offset (this is what we are fixing)
	// Start PIC1 at 32 (0x20 in hex) (IRQ0=0x20, ..., IRQ7=0x27)
	// Start PIC2 right after, at 40 (0x28 in hex)
	outb(PIC1_DATA_PORT, 0x20);
	outb(PIC2_DATA_PORT, 0x28);
	// ICW3: Cascading (how master/slave PICs are wired/daisy chained)
	// Tell PIC1 there is a slave PIC at IRQ2 (why 4? don't ask me - https://wiki.osdev.org/8259_PIC)
	// Tell PIC2 "its cascade identity" - again, I'm shaky on this concept. More resources in notes
	outb(PIC1_DATA_PORT, 0x04);
	outb(PIC2_DATA_PORT, 0x02);
	// ICW4: "Gives additional information about the environemnt"
	// See notes for some potential values
	// We are using 8086/8088 (MCS-80/85) mode
	// Not sure if that's relevant, but there it is.
	// Other modes appear to be special slave/master configurations (see wiki)
	outb(PIC1_DATA_PORT, 0x1);
	outb(PIC2_DATA_PORT, 0x1);
	// Voila! PICs are initialized

	// Mask all interrupts (why? not entirely sure)
	// 0xff is 16 bits that are all 1.
	// This masks each of the 16 interrupts for that PIC.
	outb(PIC1_DATA_PORT, 0x00);
	outb(PIC2_DATA_PORT, 0x00);
    InitIDTDesc(0, (uint32_t)isr0,0x08, 0x8E);
    InitIDTDesc(1, (uint32_t)isr1,0x08, 0x8E);
    InitIDTDesc(2, (uint32_t)isr2,0x08, 0x8E);
    InitIDTDesc(3, (uint32_t)isr3,0x08, 0x8E);
    InitIDTDesc(4, (uint32_t)isr4, 0x08, 0x8E);
    InitIDTDesc(5, (uint32_t)isr5, 0x08, 0x8E);
    InitIDTDesc(6, (uint32_t)isr6, 0x08, 0x8E);
    InitIDTDesc(7, (uint32_t)isr7, 0x08, 0x8E);
    InitIDTDesc(8, (uint32_t)isr8, 0x08, 0x8E);
    InitIDTDesc(9, (uint32_t)isr9, 0x08, 0x8E);
    InitIDTDesc(10, (uint32_t)isr10, 0x08, 0x8E);
    InitIDTDesc(11, (uint32_t)isr11, 0x08, 0x8E);
    InitIDTDesc(12, (uint32_t)isr12, 0x08, 0x8E);
    InitIDTDesc(13, (uint32_t)isr13, 0x08, 0x8E);
    InitIDTDesc(14, (uint32_t)isr14, 0x08, 0x8E);
    InitIDTDesc(15, (uint32_t)isr15, 0x08, 0x8E);
    InitIDTDesc(16, (uint32_t)isr16, 0x08, 0x8E);
    InitIDTDesc(17, (uint32_t)isr17, 0x08, 0x8E);
    InitIDTDesc(18, (uint32_t)isr18, 0x08, 0x8E);
    InitIDTDesc(19, (uint32_t)isr19, 0x08, 0x8E);
    InitIDTDesc(20, (uint32_t)isr20, 0x08, 0x8E);
    InitIDTDesc(21, (uint32_t)isr21, 0x08, 0x8E);
    InitIDTDesc(22, (uint32_t)isr22, 0x08, 0x8E);
    InitIDTDesc(23, (uint32_t)isr23, 0x08, 0x8E);
    InitIDTDesc(24, (uint32_t)isr24, 0x08, 0x8E);
    InitIDTDesc(25, (uint32_t)isr25, 0x08, 0x8E);
    InitIDTDesc(26, (uint32_t)isr26, 0x08, 0x8E);
    InitIDTDesc(27, (uint32_t)isr27, 0x08, 0x8E);
    InitIDTDesc(28, (uint32_t)isr28, 0x08, 0x8E);
    InitIDTDesc(29, (uint32_t)isr29, 0x08, 0x8E);
    InitIDTDesc(30, (uint32_t)isr30, 0x08, 0x8E);
    InitIDTDesc(31, (uint32_t)isr31, 0x08, 0x8E);

    InitIDTDesc(32, (uint32_t)irq0, 0x08, 0x8E);
    InitIDTDesc(33, (uint32_t)irq1, 0x08, 0x8E);
    InitIDTDesc(34, (uint32_t)irq2, 0x08, 0x8E);
    InitIDTDesc(35, (uint32_t)irq3, 0x08, 0x8E);
    InitIDTDesc(36, (uint32_t)irq4, 0x08, 0x8E);
    InitIDTDesc(37, (uint32_t)irq5, 0x08, 0x8E);
    InitIDTDesc(38, (uint32_t)irq6, 0x08, 0x8E);
    InitIDTDesc(39, (uint32_t)irq7, 0x08, 0x8E);
    InitIDTDesc(40, (uint32_t)irq8, 0x08, 0x8E);
    InitIDTDesc(41, (uint32_t)irq9, 0x08, 0x8E);
    InitIDTDesc(42, (uint32_t)irq10, 0x08, 0x8E);
    InitIDTDesc(43, (uint32_t)irq11, 0x08, 0x8E);
    InitIDTDesc(44, (uint32_t)irq12, 0x08, 0x8E);
    InitIDTDesc(45, (uint32_t)irq13, 0x08, 0x8E);
    InitIDTDesc(46, (uint32_t)irq14, 0x08, 0x8E);
    InitIDTDesc(47, (uint32_t)irq15, 0x08, 0x8E);

    // Last but not least, we fill out the IDT descriptor
	// and load it into the IDTR register of the CPU,
	// which is all we need to do to make it active.
	struct IDT_pointer idt_ptr;
	idt_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE) - 1;
	idt_ptr.base = (unsigned long) &IDT;
	// Now load this IDT
	LoadIDT(&idt_ptr);
}

void InitIDTDesc(uint8_t num,
	             uint32_t base,
	             uint16_t selector,
	             uint8_t flags)
{
    IDT[num].base_low = base & 0xFFFF;
    IDT[num].base_high = (base >> 16) & 0xFFFF;
    IDT[num].sel = selector;
    IDT[num].always0 = 0;
    IDT[num].flags = flags | 0x60;
}

void ISRHandler(struct InterruptRegisters* regs){
    if (regs->int_no < 32){
        printf("%s\nException! System halted.\n", exception_messages[regs->int_no]);
        for (;;);
    }
}

void* irq_routines[16] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
};

void IRQInstallHandler(int irq, void (*handler)(struct InterruptRegisters* regs)){
    irq_routines[irq] = handler;
}

void IRQRemoveHandler(int irq){
    irq_routines[irq] = 0;
}

void IRQHandler(struct InterruptRegisters* regs){
    void (*handler)(struct InterruptRegisters *regs);
    handler = irq_routines[regs->int_no - 32];
    if (handler)
        handler(regs);

    if (regs->int_no >= 40)
        outb(0xA0, 0x20);

    outb(0x20,0x20);
}
