#include <screen/screen.h>
#include <memory/memory.h>
#include <common/stdint.h>

int cursor_row = 0;
int cursor_col = 0;

extern void print_char_with_asm(char c, int row, int col);

void set_row(int row){
	cursor_row = row;
}
void set_col(int col){
	cursor_col = col;
}

void set_cords(int row, int col){
	cursor_row = row;
	cursor_col = col;
}

void println(char* string) {
	print(string);
	newline();
}

void print(char* string) {
	int i = 0;
	while (string[i] != '\0') putchar(string[i++]);
}

void safe_println(char* string, int len) {
	safe_print(string, len);
	cursor_col = 0;
	if (cursor_row == ROWS - 1) scroll();
	else cursor_row++;
}

void safe_print(char* string, int len) {
	for (int i = 0; i < len; i++) {
		printchar_at(string[i], cursor_row, cursor_col);
		if (cursor_row == ROWS - 1) scroll();
		else cursor_row++;
	}
}

void putchar(char c) {
	if (c == '\n'){
		cursor_col = 0;
		if (cursor_row == ROWS - 1) scroll();
		else cursor_row++;
		return;
	}
	if (cursor_row >= ROWS) {
		cursor_row = cursor_row % ROWS;
		// clear the row before we print
		for (int i = 0; i < COLS; i++) {
			printchar_at(' ', cursor_row, i);
		}
	}
    printchar_at(c, cursor_row, cursor_col++);
    if (cursor_col >= COLS) {
        cursor_col = cursor_col % COLS;
        if (cursor_row == ROWS - 1) scroll();
		else cursor_row++;
    }
}

void printchar_at(char c, int row, int col) {
	// OFFSET = (ROW * 80) + COL
	char* offset = (char*) (VIDMEM + 2*((row * COLS) + col));
	*offset = c;
}

void clear_screen() {
	unsigned short* ptr = (unsigned short*)VIDMEM;
	uint8_t blank = 0 | (7 & 0x0F);
	uint16_t cell = (blank << 8) | 0;
	for (int i = 0; i < 80 * 25; i++){
		ptr[i] = cell;
	}
}

void scroll() {
    unsigned char* videoMemory = (unsigned char*)VIDMEM;    
    for (int i = 0; i < ROWS - 1; i++) {
        for (int j = 0; j < COLS; j++) {
            videoMemory[i * COLS * 2 + j * 2] = videoMemory[(i + 1) * COLS * 2 + j * 2];
            videoMemory[i * COLS * 2 + j * 2 + 1] = videoMemory[(i + 1) * COLS * 2 + j * 2 + 1];
        }
    }
    for (int i = ROWS - 1; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            videoMemory[i * COLS * 2 + j * 2] = ' ';
            videoMemory[i * COLS * 2 + j * 2 + 1] = /* character color */ 0x07;  
        }
    }
}

void newline() {
    if (cursor_row == ROWS - 1) scroll();
	else cursor_row++;
    cursor_col = 0;
}

void backspace() {
    if (cursor_col > PROMPT_LENGTH) {
        print_char_with_asm(' ', cursor_row, --cursor_col);
    }
}

void print_prompt() {
	if (cursor_col != 0){
		if (cursor_row == ROWS - 1) scroll();
		cursor_col = 0;
	}
	print(PROMPT);
	cursor_col = PROMPT_LENGTH;
}

void print_message() {
	println("-HyperOS-");
}