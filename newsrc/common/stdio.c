#include <drivers/terminal/flanterm.h>
#include <memory/memory.h>
#include <common/string.h>
#include <common/stdlib.h>
#include <common/stdarg.h>
#include <common/stdio.h>

struct flanterm_context* stdout;

void printf(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	while (*fmt){
		if (*fmt != '%'){
			putchar(*fmt++);
			continue;
		}
		char buf[128] = {0};
		switch(*(fmt + 1)){
            case 'l':
                if (*(fmt + 2) != 'd') return;
                litoa(va_arg(args, int64_t), buf);
                for (int i = 0; i < strlen(buf); i++) putchar(buf[i]);
                memset(buf, 0x00, strlen(buf));
                fmt++;
                break;
			case 'd':
				itoa(va_arg(args, int), buf);
				for (int i = 0; i < strlen(buf); i++) putchar(buf[i]);
				memset(buf, 0x00, strlen(buf));
				break;
			case 'x':
				itoah(va_arg(args, int), buf);
				for (int i = 0; i < strlen(buf); i++) putchar(buf[i]);
				memset(buf, 0x00, strlen(buf));
				break;
			case 'b':
				itoab(va_arg(args, int), buf);
				for (int i = 0; i < strlen(buf); i++) putchar(buf[i]);
				memset(buf, 0x00, strlen(buf));
				break;
			case 's':;
				char* str = va_arg(args, char*);
				for (int i = 0; i < strlen(str); i++) putchar(str[i]);
				break;
		}
		fmt += 2;
	}
}