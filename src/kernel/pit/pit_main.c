#include <common/stdint.h>
#include <kernel/pit/pit.h>
#include <kernel/kernel.h>
#include <kernel/idt/idt.h>
#include <common/stdio.h>

volatile uint64_t ticks;
const uint32_t freq = 100;

void OnIRQ0(struct InterruptRegisters *regs){
    ticks += 1;
    outb(PIC1_COMMAND_PORT, 0x20);
}

void InitTimer(){
    ticks = 0;
    IRQInstallHandler(0, &OnIRQ0);

    //119318.16666 Mhz
    uint32_t divisor = 1193180/freq;

    //0011 0110
    outb(0x43,0x36);
    outb(0x40,(uint8_t)(divisor & 0xFF));
    outb(0x40,(uint8_t)((divisor >> 8) & 0xFF));
}

void Sleep(uint32_t ms){
    InterruptsEnable();
    uint64_t target = ticks + (uint64_t)(ms / 10);
    while (ticks < target);
}

uint64_t GetTickCount(void){ return ticks; }