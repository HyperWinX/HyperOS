#pragma once

#include <drivers/terminal/flanterm.h>

#define puts(str) flanterm_write(stdout, str, sizeof(str))
#define putchar(c) flanterm_putchar(stdout, c)

extern struct flanterm_context* stdout;

void printf(const char* fmt, ...);