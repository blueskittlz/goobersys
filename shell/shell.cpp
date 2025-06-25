extern "C" {
    void vga_write(const char* str);
    char keyboard_read_char();
}

#include "lib/string.h"

struct Args {
    int count;
    char args[10][32];
};

void split(const char* input, Args& out) {
    out.count = 0;
    int arg_idx = 0, char_idx = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            if (char_idx > 0) {
                out.args[out.count][char_idx] = '\0';
                out.count++;
                char_idx = 0;
                if (out.count >= 10) break;
            }
        } else {
            if (char_idx < 31) {
                out.args[out.count][char_idx++] = input[i];
            }
        }
    }
    if (char_idx > 0 && out.count < 10) {
        out.args[out.count][char_idx] = '\0';
        out.count++;
    }
}

void cmd_clear(const Args&) {
    vga_write("\033[2J\033[H"); // clear screen and move cursor home
}

void cmd_echo(const Args& args) {
    for (int i = 1; i < args.count; i++) {
        vga_write(args.args[i]);
        vga_write(" ");
    }
    vga_write("\n");
}

void run_command(const Args& args) {
    if (args.count == 0) return;

    if (strcmp(args.args[0], "clear") == 0) {
        cmd_clear(args);
    } else if (strcmp(args.args[0], "echo") == 0) {
        cmd_echo(args);
    } else {
        vga_write("Unknown command\n");
    }
}

void shell_loop() {
    char input[256];
    int pos = 0;

    vga_write("> ");

    while (true) {
        char c = keyboard_read_char();
        if (c == '\r') {
            input[pos] = '\0';
            vga_write("\n");
            Args args;
            split(input, args);
            run_command(args);
            pos = 0;
            vga_write("> ");
        } else if (c == '\b') {
            if (pos > 0) {
                pos--;
                // TODO: handle backspace on screen
            }
        } else if (pos < 255) {
            input[pos++] = c;
            char s[2] = {c, 0};
            vga_write(s);
        }
    }
}
