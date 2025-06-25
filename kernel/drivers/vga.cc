#include <stdint.h>

volatile uint16_t* VGA_MEMORY = (volatile uint16_t*)0xB8000;
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;

static int cursor_row = 0;
static int cursor_col = 0;

static uint8_t color = 0x07; // Light grey on black

void vga_clear() {
    uint16_t blank = (color << 8) | ' ';
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = blank;
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_put_char(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_HEIGHT) {
            cursor_row = 0; // or scroll later
        }
        return;
    }
    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = (color << 8) | c;
    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_HEIGHT) {
            cursor_row = 0; // or scroll later
        }
    }
}

void vga_write(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_put_char(str[i]);
    }
}
