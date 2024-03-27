#ifndef PIT_H
#define PIT_H

#include <common/stdint.h>
#include <kernel/idt/idt.h>

void InitTimer();
void OnIRQ0(struct InterruptRegisters* regs);
void Sleep(uint32_t ms);
uint64_t GetTickCount(void);

#endif