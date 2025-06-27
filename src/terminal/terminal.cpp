#include <Uefi.h>
#include "../framebuffer/framebuffer.h"
#include "../common/uefi_shims.h"
#include "terminal.h"
#include "../common/globals.h"
// Terminal size constants
#define MAX_TERMINAL_LINES 25
#define MAX_LINE_LENGTH 80
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

// Terminal buffer
static CHAR16 terminalBuffer[MAX_TERMINAL_LINES][MAX_LINE_LENGTH + 1];
static int currentLine = 0;
static int currentPos = 0;

// Simple 8x16 bitmap font (only for ASCII 32..127)
// Each character is 16 bytes (one byte per row, 8 pixels wide)
// This font array is a placeholder; you need to fill it with real bitmap data.
// For brevity, I'll provide a dummy font with all blank chars. You should replace it with a real font.
static const UINT8 font8x16[96][16] = { {0} };

// Draw a character at framebuffer position (x, y) with white pixels on black background
void DrawChar(int x, int y, CHAR16 c) {
    if (c < 32 || c > 127) c = '?'; // fallback for unsupported chars

    const UINT8* glyph = font8x16[c - 32];
    for (int row = 0; row < CHAR_HEIGHT; row++) {
        UINT8 bits = glyph[row];
        for (int col = 0; col < CHAR_WIDTH; col++) {
            UINT32 color = (bits & (1 << (7 - col))) ? 0xFFFFFFFF : 0x00000000; // white or transparent
            PutPixel(x + col, y + row, color);
        }
    }
}

// Clear terminal area (fill with black)
void ClearTerminalArea(int x, int y, int width, int height) {
    DrawRect(x, y, width, height, 0x000000);
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
            // Scroll up
            for (int i = 1; i < MAX_TERMINAL_LINES; i++) {
                for (int j = 0; j < MAX_LINE_LENGTH; j++) {
                    terminalBuffer[i - 1][j] = terminalBuffer[i][j];
                }
            }
            currentLine = MAX_TERMINAL_LINES - 1;
            for (int i = 0; i < MAX_LINE_LENGTH; i++) {
                terminalBuffer[currentLine][i] = L' ';
            }
        }
    } else if (currentPos < MAX_LINE_LENGTH) {
        terminalBuffer[currentLine][currentPos] = c;
        currentPos++;
        terminalBuffer[currentLine][currentPos] = L'\0';
    }
}

// Draw entire terminal buffer inside given area (x,y,width,height)
void TerminalDraw(int x, int y) {
    // Clear background
    ClearTerminalArea(x, y, MAX_LINE_LENGTH * CHAR_WIDTH, MAX_TERMINAL_LINES * CHAR_HEIGHT);

    for (int line = 0; line <= currentLine; line++) {
        int px = x;
        int py = y + line * CHAR_HEIGHT;

        for (int col = 0; terminalBuffer[line][col] != L'\0'; col++) {
            DrawChar(px + col * CHAR_WIDTH, py, terminalBuffer[line][col]);
        }
    }
}
