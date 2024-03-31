#pragma once

#include <common/stdint.h>

// Defined character sizes
#define CHARWIDTH 6
#define CHARHEIGHT 10

// Terminal structure, contains all information required for terminal to work
typedef struct terminal_t{
    uint16_t width;
    uint16_t height;
    uint8_t bpp;
    void* address;
    uint32_t total_size;
    uint32_t foreground_clr;
    uint32_t background_clr;
    uint16_t line_length;
} terminal_t;