#include <boot/limine.h>
#include <common/stdio.h>
#include <drivers/terminal/flanterm.h>
#include <drivers/terminal/fb.h>

// Limine framebuffer request
static volatile struct limine_framebuffer_request kframebuffer = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void _start(void) {
    // Init variables and framebuffer
    struct limine_framebuffer* framebuffer = kframebuffer.response->framebuffers[0];
    struct flanterm_context* ft_ctx;
    const char hello[] = "Hello World from HyperOS, based on Limine and FlanTerm!";

    // Init terminal
    ft_ctx = flanterm_fb_simple_init(framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch);
    flanterm_write(ft_ctx, hello, sizeof(hello));
    while (1);
}