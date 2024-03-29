#ifndef CPUID_H
#define CPUID_H

#define ManufacturerOnly 1

extern char cpu_bs[49];
extern char manufacturer[13];
extern char instructions_supported[8];
extern void RunCPUID(void);

#endif