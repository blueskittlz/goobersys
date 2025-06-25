#include <stdint.h>

// Forward declare driver functions (extern "C" if needed)
extern "C" {
    void vga_clear();
    void vga_write(const char* str);
    uint8_t keyboard_read_scancode();
    char scancode_to_ascii(uint8_t scancode);
}

char keyboard_read_char() {
    uint8_t scancode = keyboard_read_scancode();
    return scancode_to_ascii(scancode);
}

extern "C" void kernel_main() {
    vga_clear();
    vga_write("Simple OS Kernel\nType keys:\n");

    while (true) {
        char c = keyboard_read_char();
        if (c) {
            if (c == '\r') {
                vga_write("\n");
            } else {
                char str[2] = {c, 0};
                vga_write(str);
            }
        }
    }
}
