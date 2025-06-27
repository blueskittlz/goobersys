#ifndef TERMINAL_H
#define TERMINAL_H

#include <Uefi.h>
#include "../framebuffer/framebuffer.h"
#include "../common/uefi_shims.h"
#include "../common/globals.h"
// Add a character to the terminal buffer
void TerminalPutChar(CHAR16 c);

// Draw the terminal contents at (x, y)
void TerminalDraw(int x, int y);

// Poll for a key press, returns true if a key was received and stored in outChar
bool PollKey(CHAR16* outChar);

#endif // TERMINAL_H