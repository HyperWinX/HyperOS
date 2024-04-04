static char vendor[13];
static char cpu_brandstring[49];

extern void _read_vendor(void);
extern void _read_cpu_brandstring(void);

void get_cpuid_vendorstring(void){
    _read_vendor();
    _read_cpu_brandstring();
}