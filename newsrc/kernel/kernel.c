#include <boot/limine.h>
#include <boot/init.h>
#include <common/stdio.h>
#include <drivers/terminal/flanterm.h>
#include <drivers/terminal/fb.h>

#define init_stdout() stdout = flanterm_fb_simple_init(framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch)

void get_cpuid_vendorstring(void);

// Defined strings
extern char vendor[];
extern char cpu_brandstring[];
static const char term_init_msg[] = "kernel: FlanTerm initialized\n";
static const char fpu_init_start[] = "kernel: trying to set up FPU...   ";
static const char fpu_init_success[] = "[ OK ]\n";
static const char fpu_init_fail[] = "[FAIL]\n";

// Limine framebuffer request
static volatile struct limine_framebuffer_request kframebuffer = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};



void _start(void) {
    // Init variables and framebuffer
    struct limine_framebuffer* framebuffer = kframebuffer.response->framebuffers[0];
    if (framebuffer == NULL) while (1);

    // Init terminal
    init_stdout();
    puts(term_init_msg);

    // Try to init FPU
    puts(fpu_init_start);
    if (fpu_exists() == 1){
        init_fpu();
        puts(fpu_init_success);
    }
    else puts(fpu_init_fail);
    
    // Read CPUID
    get_cpuid_vendorstring();
    printf("kernel: CPU vendor: %s\n", vendor);
    while (1);
}