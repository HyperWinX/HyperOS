#include <drivers/terminal/terminal.h>
#include <fonts/pixelate.h>
#include <memory/memory.h>
#include <common/stdint.h>
#include <boot/limine.h>

int cursor_row = 0;
int cursor_col = 0;

terminal_t current_term;

void hterminal_init(struct limine_framebuffer* framebuffer){
	current_term.width = framebuffer->width;
	current_term.height = framebuffer->height;
	current_term.bpp = framebuffer->bpp;
	current_term.address = framebuffer->address;
	current_term.total_size = framebuffer->height * framebuffer->width * (framebuffer->bpp / 8);
	current_term.foreground_clr = 0x00FFFFFF;
	current_term.background_clr = 0x00000000;
	current_term.line_length = framebuffer->width * (framebuffer->bpp / 8);
}

void hterminal_clear(void){
	memset(current_term.address, 0x00, current_term.total_size);
}

void hterminal_printchar(uint8_t charcode){
	uint8_t shift_factor = 60;
	uint64_t character = font[charcode];

	for (int i = 0; i < CHARHEIGHT; ++i){
		void* address = current_term.address + (current_term.line_length * i);
		for (int j = 0; j < CHARWIDTH; ++j){
			// Calculate address of pixel and define, fill it or not
			*(uint32_t*)(current_term.address + (current_term.line_length * i) + (CHARWIDTH * j)) = (uint8_t)(character >> shift_factor ? current_term.foreground_clr : current_term.background_clr);
			// Decrease shift factor
			shift_factor--;
		}
		address += current_term.line_length;
	}
}