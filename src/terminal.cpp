#include <Uefi.h>
#include "uefi_shims.h"
#include "framebuffer.h"

#define MAX_TERMINAL_LINES 25
#define MAX_LINE_LENGTH 80

static CHAR16 terminalBuffer[MAX_TERMINAL_LINES][MAX_LINE_LENGTH + 1];
static int currentLine = 0;
static int currentPos = 0;

// Shit is annyoing asf hope this fixes it [praying emoji]
void UefiStrCpy(CHAR16* dest, const CHAR16* src) {
    while ((*dest++ = *src++));
}

// Add a character to the terminal buffer
void TerminalPutChar(CHAR16 c) {
    if (c == L'\b') { // Backspace
        if (currentPos > 0) {
            currentPos--;
            terminalBuffer[currentLine][currentPos] = L' ';
        }
    } else if (c == L'\n' || c == L'\r') {
        currentLine++;
        currentPos = 0;
        if (currentLine >= MAX_TERMINAL_LINES) {
            // AY TWIN SCROLL UP
            for (int i = 1; i < MAX_TERMINAL_LINES; i++) {
                UefiStrCpy(terminalBuffer[i - 1], terminalBuffer[i]);
            }
            currentLine = MAX_TERMINAL_LINES - 1;
            for (int i = 0; i < MAX_LINE_LENGTH; i++) {
                terminalBuffer[currentLine][i] = L' ';
            }
            terminalBuffer[currentLine][0] = L'\0';
        }
    } else if (currentPos < MAX_LINE_LENGTH) {
        terminalBuffer[currentLine][currentPos] = c;
        currentPos++;
        terminalBuffer[currentLine][currentPos] = L'\0';
    }
}

// Draw terminal contents using UEFI Print for now
void TerminalDraw(int x, int y) {
    for (int i = 0; i <= currentLine; i++) {
        TPrint(terminalBuffer[i]);
        TPrint(UEFI_STR("\r\n"));
    }
}