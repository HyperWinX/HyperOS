#include <boot/limine.h>
#include <common/stdio.h>
#include <drivers/terminal/terminal.h>

// Limine framebuffer request
static volatile struct limine_framebuffer_request kframebuffer = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void _start(void) {
    struct limine_framebuffer* framebuffer = kframebuffer.response->framebuffers[0];
    // Terminal initialization
    hterminal_init(framebuffer);
    hterminal_clear();

    hterminal_printchar(33);

}