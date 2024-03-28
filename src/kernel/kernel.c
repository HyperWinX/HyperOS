#include <kernel/kernel.h>
#include <kernel/keyboard_map.h>
#include <common/stdlib.h>
#include <screen/screen.h>
#include <drivers/vga/vga.h>
#include <drivers/pci/pci.h>
#include <common/stdio.h>
#include <kernel/pit/pit.h>
#include <kernel/idt/idt.h>
#include <kernel/cpuid/cpuid.h>

// ----- Global variables -----
// This is our entire IDT. Room for 256 interrupts

char command_buffer[COMMAND_BUFFER_SIZE];
int command_len = 0;

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void KeyboardInitialize() {
    IRQInstallHandler(1, &KeyboardKeyPress);
}

void KeyboardKeyPress(struct InterruptRegisters* regs) {
    // Write end of interrupt (EOI)
    outb(PIC1_COMMAND_PORT, 0x20);

    unsigned char status = inb(KEYBOARD_STATUS_PORT);
    // Lowest bit of status will be set if buffer not empty
    // (thanks mkeykernel)
    if (status & 0x1) {
        char keycode = inb(KEYBOARD_DATA_PORT);
        if (keycode < 0 || keycode >= 128) return;
        if (keycode == 28) {
            // ENTER : Newline
            newline();
            // null terminate command buffer
            command_buffer[command_len] = '\0';
            // Handle command
            if (strcmp(command_buffer, "ls")) {
                println("Filesystem not yet implemented.");
            } else if (strcmp(command_buffer, "clear")) {
                clear_screen();
            } else if (strcmp(command_buffer, "vga")) {
                vga_enter();
                draw_happy_face(5, 5);
            } else if (strcmp(command_buffer, "vgainfo")) {
                vga_info();
            } else if (strcmp(command_buffer, "lspci")) {
                lspci();
            } else if (strcmp(command_buffer, "test")){
                printf("Local test, bruh\n");
                printf("Total ticks: %d\n", GetTickCount());
                printf("Waiting second\n");
                Sleep(1000);
                printf("Second passed\n");
            } else if (strcmp(command_buffer, "scroll")) {
                scroll();
            } else if (strcmp(command_buffer, "poweroff")){
                clear_screen();
                __asm__ volatile("hlt");
            } else if (strcmp(command_buffer, "help")) {
                println("clear: Clear screen");
                println("ls: List files");
                println("lspci: Enumerate PCI devices");
                println("ide: Run ide test");
                println("vga: Run VGA test");
                println("vgainfo: Print VGA info");
            } else if (command_len < 1) {
                // do nothing
            } else {
                print("Command not found: ");
                println(command_buffer);
                println("Write `help` to see commands.");
            }
            command_len = 0;
            print_prompt();
        } else if (keycode == 14) {
            // BACKSPACE: Move back one unless on prompt
            backspace();
            if (command_len > 0) {
                command_len--;
            }
        } else if (keycode == 1) {
            // ESCAPE
            vga_exit();
        } else {
            if (command_len >= COMMAND_BUFFER_SIZE) return;
            command_buffer[command_len++] = keyboard_map[(int)keycode];
            putchar(keyboard_map[(int)keycode]);
        }
    }
}

// ----- Entry point -----
int kmain() {
    //disable_cursor();
    //KeyboardInitialize();
    InitIDT();
    InterruptsEnable();
    //RunCPUID();
    //InitTimer();
    clear_screen();
    print_message();
    printf("\nBuild date: %s, build_time: %s\n", __DATE__, __TIME__);
    printf("CPU: %s\n", cpu_bs);
    printf("Manufacturer: %s\n", manufacturer);
    //Sleep(100);
    //InitIDE(0, 0, 0, 0, 0);
    print_prompt();
    // Finish main execution, but don't halt the CPU. Same as `jmp $` in assembly
    for (;;);
}