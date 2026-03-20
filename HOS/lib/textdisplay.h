/* ===============================
    (c) 2026 hommemovie corpration. all rights reserved.
    ==============================
    please do not use or distribute this code without permission even if its on github. its supposed to be there.
        - hommemovie
        ==============================
        This is the main kernel code for H-OS, a minimal 64-bit operating system. It includes basic VGA text output, keyboard input handling, and a simple shell with a few commands. The kernel is designed to be as small and simple as possible while still providing a functional environment for users to interact with.
        ==============================
        Note: This code is intended for educational purposes and may not be suitable for production use. It lacks many features of a full operating system, such as memory management, multitasking, and file systems.
        ===============================
        also anyone can help with the project just ask update it.
        i just realized is it 10 pm?
    who is coding at 10:55 pm? its 10:30 pm. i should go to sleep. good night.
    hell nah i just woke up. its 10:55 pm no quitting coding now. this kernel has to be done by the end of the week. i have to finish it.
    im just weird....
    ============================ */

    #ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <stdint.h>
#include <stddef.h>

// VGA text mode buffer address
static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

// Text colors
enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

// Helper to combine foreground and background
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Helper to create a character entry
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

// Initialize terminal
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
            VGA_BUFFER[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Set text color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put a single character
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) terminal_row = 0; // Simple wrapping
        return;
    }
    const int index = terminal_row * VGA_WIDTH + terminal_column;
    VGA_BUFFER[index] = vga_entry(c, terminal_color);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) terminal_row = 0;
    }
}

// Write a string
void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        terminal_putchar(data[i]);
    }
}

#endif
