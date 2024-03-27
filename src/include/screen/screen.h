#ifndef SCREEN_H
#define SCREEN_H

#define ROWS 25
#define COLS 80
#define VIDMEM 0xB8000
#define VIDMEMEND 0xB8FA0
#define PROMPT "HyperOS> "
#define PROMPT_LENGTH 9

void println(char* string);
void print(char* string);
void safe_println(char* string, int len);
void safe_print(char* string, int len);
void putchar(char c);
void printchar_at(char c, int row, int col);
//extern void print_char_with_asm(char c, int row, int col);
void clear_screen();
void print_prompt();
void print_message();
void newline();
void backspace();
void scroll();

#endif